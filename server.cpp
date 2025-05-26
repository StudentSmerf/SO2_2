#include "common.h"
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>

void handle_client(pid_t pid) {
    std::string shmName = shm_name(pid);
    std::string semClientName = sem_client_name(pid);
    std::string semServerName = sem_server_name(pid);

    int shm_fd = shm_open(shmName.c_str(), O_RDWR, 0666);
    auto* data = (SharedData*) mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_t* sem_client = sem_open(semClientName.c_str(), 0);
    sem_t* sem_server = sem_open(semServerName.c_str(), 0);

    std::cout << "[*] Połączono z klientem " << pid << std::endl;

    while (true) {
        sem_wait(sem_client);

        std::cout << "[Klient " << pid << "]: " << data->message << std::endl;

        std::string response = "Otrzymałem: ";
        response += data->message;
        strncpy(data->message, response.c_str(), MAX_MSG_LEN);

        sem_post(sem_server);
    }
}

int main() {
    mkfifo(ANNOUNCE_FIFO, 0666);
    std::ifstream fifo(ANNOUNCE_FIFO);

    std::vector<std::thread> threads;

    std::cout << "Serwer oczekuje klientów...\n";

    std::string line;
    while (std::getline(fifo, line)) {
        pid_t pid = std::stoi(line);
        threads.emplace_back(handle_client, pid);
    }

    for (auto& t : threads)
        t.join();

    return 0;
}
