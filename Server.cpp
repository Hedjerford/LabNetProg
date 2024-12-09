#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <cstring>

#define ln '\n'
#define REQUEST_ERR "Некорректный запрос. Попробуйте ещё раз."
#define LEN 200



const char* path = "/tmp/path";

void print(std::vector <int> &a) {
	for (int i = 0; i < a.size(); ++i) {
		std::cout << a[i] << ' ';
	}
	std::cout << ln;
}


int main() { 
	srand(time(nullptr));
	//unlink(path);
    if (mkfifo(path, 2034) == -1) {
        if (errno == EEXIST) {
            std::cerr << "Ошибка: канал уже существует." << ln;
            return 1;
        } else {
            std::cerr << "Ошибка создания канала: " << ln;
            return 1;
        }
    }

    std::string message;
    char buffer[LEN];
	bool SORT = 1;
	bool client = 0;
	int fd;
	int count = 0;
	bool setMessage= 0;
	int n = -1;
	std::vector <int> arr;
	int start = 5, end = 10;

	memset(buffer, '\0', sizeof(buffer));

    while (1) {
		fd = open(path, O_RDONLY);
		ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
	
		if (bytes_read <= 0) { 
			std::cerr<< "Ошибка чтения из канала." << ln;
			close(fd);
			break;
		}			
		
		message = buffer;
		std::cout << "Клиент: " << message << ln;
		close(fd);
		if (message == "exit")  {
			break;
		}

		memset(buffer, '\0', sizeof(buffer));
		fd = open(path, O_WRONLY);
		
		if (message == "start") {
			if (!client) {
				client = 1;
				if (n == -1) {
					n = rand() % (end - start + 1) + start;
					message = std::to_string(n);
					ssize_t bytes_written = write(fd, message.c_str(), message.length());
					if (bytes_written <= 0) {
						std::cerr << "Ошибка записи в канал." <<  ln;
						break;
					}
					close(fd);
					for (int i = 0; i < n; ++i) {
						arr.push_back(rand() % 3);
					}
					print(arr);
					continue;
				}	
			}
			else {
				message = "-Уже имеется 1 подключенный пользователь. Сервер перегружен.";
				std::cerr << message;
				ssize_t bytes_written = write(fd, message.c_str(), message.length());
				if (bytes_written <= 0) {
					std::cerr << "Ошибка записи в канал." <<  ln;
					break;
				}
				close(fd);
				fd = open(path, O_WRONLY);
				break;
			}
		}
		if (client) {
			
			if (message == "sorted") {
				for (int i = 1; i < n; ++i) {
					if (arr[i] < arr[i - 1]) {
						SORT = 0;
						break;
					}
				}
				if (SORT) {
					message = "-Задача успешно решена!";
				}
				else {
					message = "-Массив ещё не отсортирован! Программа завершает работу."; 
				}
				ssize_t bytes_written = write(fd, message.c_str(), message.length());
				if (bytes_written <= 0) {
					std::cerr << "Ошибка записи в канал." <<  ln;
					break;
				}
				close(fd);
				break;
			}

			if (message == "-1") {
				message = "-Tusk failed successfully!";
				ssize_t bytes_written = write(fd, message.c_str(), message.length());
				if (bytes_written <= 0) {
					std::cerr << "Ошибка записи в канал." <<  ln;
					break;
				}
				close(fd);
				break;
			}
			std::vector <std::string> temp;
			std::stringstream ss(message);
			int type, i, j, id = 0; 
			std::string s;
			bool flag = 1;
			while (ss >> s) {
				temp.push_back(s);
				if (temp.size() > 3) {
					flag = 0;
					break;
				}
			}
			if (flag) {
				if (temp[0].size() > 1) {
					flag = 0;
				}
				else {
					if (stoi(temp[0]) > 0 && stoi(temp[0]) < 3) {
						if (temp[0] == "1") {
							if (temp.size() != 2) {
								flag = 0;
							}
							else {
								bool f = 1; 
								for (int i = 0; i < temp[0].size(); ++i) {
									if (temp[0][i] < 48 || temp[0][j] > 57) {
										f = 0;
										break;
									}
								}
								if (f) {
									if (count == 2 * n) {
										message = "-Вы превысили лимит операций. Программа завершает работу.";
										count = -1;
									}
									else {
										++count;
										if (stoi(temp[1]) < n && stoi(temp[1]) > -1) {
											message = std::to_string(arr[stoi(temp[1])]);
										}
										else {
											flag = 0;
										}
									}
								}
								else {
									flag = 0;
								}
							}
						}
						else {
							if (temp.size() != 3) {
								flag = 0;
							}
							else {
								bool f = 1;
								for (int i = 1; i < 3; ++i) {
									for (int j = 0; j < temp[1].size(); ++j) {
									if (temp[i][j] < 48 || temp[i][j] > 57) {
										f = 0;
										break;
										}
									}
								}
								if (f) {
									int i = stoi(temp[1]), j = stoi(temp[2]);
									if (i > -1 && i < n && j > -1 && j < n) {
										message = "swapped";
										if (count == 2 * n) {
											message = "-Вы превысили лимит операций. Программа завершает работу.";
											count = -1;
										}
										else {
											++count;
											std::swap(arr[i], arr[j]);
											print(arr);
										}
									}
									else {
										flag = 0;
									}
								}
								else {
									flag = 0;
								}
							}
						}
					}
					else {
						flag = 0;
					}
				}
			}
			if (!flag) {
				message = REQUEST_ERR;
			}
		}
		else {
			message = "Введите 'start' для начала работы.";
		}
		ssize_t bytes_written = write(fd, message.c_str(), message.length());
		if (bytes_written <= 0) {
			std::cerr << "Ошибка записи в канал." <<  ln;
			break;
		}
		close(fd);
		if (count == -1) {
			break;
		}
 	}
	unlink(path);
    std::cout << "Сервер завершил работу." << ln;
    return 0;
}