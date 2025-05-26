#pragma once
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>

#define MAX_MSG_LEN 256
#define ANNOUNCE_FIFO "/tmp/chat_announce"

struct SharedData {
    char message[MAX_MSG_LEN];
};

inline std::string shm_name(pid_t pid) {
    return "/chat_shm_" + std::to_string(pid);
}

inline std::string sem_client_name(pid_t pid) {
    return "/sem_client_" + std::to_string(pid);
}

inline std::string sem_server_name(pid_t pid) {
    return "/sem_server_" + std::to_string(pid);
}
