#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>

#define ln '\n'
#define LEN 200


const char* path = "/tmp/path";
constexpr int inf = (int)1e9;


void print(std::vector <int> &a) {
	for (int i = 0; i < a.size(); ++i) {
		std::cout << a[i] << ' ';
	}
	std::cout << ln;
}

void userSolve() {
	char ans[LEN];
	memset(ans, '\0', sizeof(ans));
	bool start = 0;
	std::string message;
	int fd;

	while (1) {
		fd = open(path, O_WRONLY);
		if (fd == -1) {
			std::cerr << "Ошибка открытия канала." <<  ln;
			return;
		}
		
		std::cout << "Введите сообщение (exit для выхода): ";
		std::getline(std::cin, message);
		
		if (message == "start") {
			if (!start) {
				start = 1;
			}
			else {
				while (1) {
					std::cout << "Вы уже начали работу программы. Пожалуйста, введите один из запросов." << ln;
					std::getline(std::cin, message);
					if (message != "start") {
						break;
					}
				}
			}
		}	
		if (message.size() > LEN) {
			std::cout << "Сообщение должно состоять не более чем из "<< LEN << " символов." << ln;
			close(fd);
			continue;
		}
		
		ssize_t bytes_written = write(fd, message.c_str(), message.length());
		if (bytes_written <= 0) {
			std::cerr << "Ошибка записи в канал." <<  ln;
			break;
		}
		close(fd);
		if (message == "exit") break;
		
		

		fd = open(path, O_RDONLY);
		ssize_t bytes_read = read(fd, ans, LEN);
		if (bytes_written <= 0) {
		 	std::cerr << "Ошибка чтения: " <<  ln;
		 	break;
		}
		message = ans;
		
		std::cout << "Сервер: " << ans << ln;
		if (message == "start") {
			close(fd);
			break;
		}
		memset(ans, '\0', sizeof(ans));
		close(fd);
		if (message[0] == '-') {
			break;
		}
	}
}

void SWAP(int i, int j) {
	std::cout << i << " " << j << ln;
	char ans[LEN];
	memset(ans, '\0', sizeof(ans));
	bool start = 0;
	std::string message = "2 " + std::to_string(i) + " " + std::to_string(j) + "\n";
	int fd;

	fd = open(path, O_WRONLY);
	if (fd == -1) {
		std::cerr << "Ошибка открытия канала." <<  ln;
		return;
	}
	
	ssize_t bytes_written = write(fd, message.c_str(), message.length());
	if (bytes_written <= 0) {
	 	std::cerr << "Ошибка записи в канал." <<  ln;
		return;
	}
	close(fd);
	
	fd = open(path, O_RDONLY);
	ssize_t bytes_read = read(fd, ans, LEN);
	if (bytes_written <= 0) {
	 	std::cerr << "Ошибка чтения." <<  ln;
		return;
	}
	message = ans;
	std::cout << "Сервер: " << ans << ln;
	memset(ans, '\0', sizeof(ans));
	close(fd);

}

void smartSolve() {
	char ans[LEN];
	memset(ans, '\0', sizeof(ans));
	bool start = 0;
	std::string message = "start";
	int fd;

	fd = open(path, O_WRONLY);
	if (fd == -1) {
		std::cerr << "Ошибка открытия канала." <<  ln;
		return;
	}
	
	ssize_t bytes_written = write(fd, message.c_str(), message.length());
	if (bytes_written <= 0) {
	 	std::cerr << "Ошибка записи в канал." <<  ln;
		return;
	}
	close(fd);
	
	fd = open(path, O_RDONLY);
	ssize_t bytes_read = read(fd, ans, LEN);
	if (bytes_written <= 0) {
	 	std::cerr << "Ошибка чтения." <<  ln;
		return;
	}
	message = ans;
	std::cout << "Сервер: " << ans << ln;
	memset(ans, '\0', sizeof(ans));
	close(fd);
	int n = std::stoi(message);
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
		std::cout << i;
        fd = open(path, O_WRONLY);
		if (fd == -1) {
			std::cerr << "Ошибка открытия канала." <<  ln;
			return;
		}
		message = "1 " + std::to_string(i) + "\n";
		ssize_t bytes_written = write(fd, message.c_str(), message.length());
		if (bytes_written <= 0) {
			std::cerr << "Ошибка записи в канал." <<  ln;
			break;
		}
		close(fd);
		
		fd = open(path, O_RDONLY);
		ssize_t bytes_read = read(fd, ans, LEN);
		if (bytes_written <= 0) {
			std::cerr << "Ошибка чтения." <<  ln;
			break;
		}
		message = ans;
		std::cout << " : " << ans << ln;
		memset(ans, '\0', sizeof(ans));
		close(fd);
		a[i] = stoi(message);
    }
    std::vector<int> b = a;
    sort(b.begin(), b.end());
    std::vector <std::set <int>> s(3);
    int s1 = inf, s2 = inf;
    for (int i = 0; i < n; ++i) {
        if (s1 == inf && b[i] == 1) {
            s1 = i;
        }
        if (s2 == inf && b[i] == 2) {
            s2 = i;
        }
        if (a[i] != b[i]) {
            s[a[i]].insert(i);
        }
    }
    
    while (s[0].size()) {
        int x = *s[0].begin();
        if (s[1].size()) {
            int y = *s[1].begin();
            if (y < s1 && x < s2) {
				SWAP(x, y);
                s[0].erase(s[0].begin());
                s[1].erase(s[1].begin());
                continue;
            }
            if (s[2].size()) {
                int z = *s[2].begin();
                if (x > s2 && z < s1) {
					SWAP(x, z);
                    s[0].erase(s[0].begin());
                    s[2].erase(s[2].begin());
                    continue;
                }
                else {
                    if (z < s1) {
						SWAP(x, z);
                        s[0].erase(s[0].begin());
                        s[2].erase(s[2].begin());
                        s[2].insert(x);
                        continue;
                    }
                }
            }
			SWAP(x, y);
            s[0].erase(s[0].begin());
            s[1].erase(s[1].begin());
            s[1].insert(x);
            continue;
        }
        int z = *s[2].begin();
        if (x > s2 && z < s1) {
			SWAP(x, z);
            s[0].erase(s[0].begin());
            s[2].erase(s[2].begin());
            continue;
        }
        else {
            if (z < s1) {
				SWAP(x, z);
                s[0].erase(s[0].begin());
                s[2].erase(s[2].begin());
                s[2].insert(x);
                continue;
            }
        }
    }
    while (s[1].size()) {
        int x = *s[1].begin();
        int y = *s[2].begin();
		SWAP(x, y);
        s[1].erase(s[1].begin());
        s[2].erase(s[2].begin());
    }
    
    message = "sorted";
	fd = open(path, O_WRONLY);
	if (fd == -1) {
		std::cerr << "Ошибка открытия канала." <<  ln;
		return;
	}
	
	bytes_written = write(fd, message.c_str(), message.length());
	if (bytes_written <= 0) {
	 	std::cerr << "Ошибка записи в канал." <<  ln;
		return;
	}
	close(fd);
	
	fd = open(path, O_RDONLY);
	bytes_read = read(fd, ans, LEN);
	if (bytes_written <= 0) {
	 	std::cerr << "Ошибка чтения." <<  ln;
		return;
	}
	message = ans;
	std::cerr << "Сервер." << ans << ln;
	memset(ans, '\0', sizeof(ans));
	close(fd);
}

int main(int argc, char *argv[]) {

	if (*argv[1] == '1') {
		userSolve();
	}
	else {
		if (*argv[1] == '2') {
			smartSolve();
		}
		else {
			std::cout << "Неправильно заданы аргументы." << ln;
			return 1;
		}
	}
	std::cout << "Клиент завершил работу." << ln;
	return 0;
}
