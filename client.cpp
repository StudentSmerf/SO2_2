#include "common.h"
#include <fstream>

int main() {
    pid_t pid = getpid();
    std::string shmName = shm_name(pid);
    std::string semClientName = sem_client_name(pid);
    std::string semServerName = sem_server_name(pid);

    int shm_fd = shm_open(shmName.c_str(), O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedData));
    auto* data = (SharedData*) mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_t* sem_client = sem_open(semClientName.c_str(), O_CREAT, 0666, 0);
    sem_t* sem_server = sem_open(semServerName.c_str(), O_CREAT, 0666, 0);

    // Zgłoś się do serwera
    std::ofstream fifo(ANNOUNCE_FIFO, std::ios::out | std::ios::app);
    fifo << pid << std::endl;

    std::string input;
    while (true) {
        std::cout << "Ty: ";
        std::getline(std::cin, input);

        strncpy(data->message, input.c_str(), MAX_MSG_LEN);
        sem_post(sem_client);

        sem_wait(sem_server);
        std::cout << "Serwer: " << data->message << std::endl;
    }

    return 0;
}
