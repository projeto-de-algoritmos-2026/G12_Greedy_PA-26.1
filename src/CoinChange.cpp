#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;


pair<int, vector<int>> coin_change(int Q, const vector<int>& cs) {
    int ans = 0;
    vector<int> usadas;

    for (auto it = cs.rbegin(); it != cs.rend(); ++it) {
        int c = *it;
        int k = Q / c;

        ans += k;

        for (int i = 0; i < k; i++)
            usadas.push_back(c);

        Q -= k * c;
    }

    return {ans, usadas};
}


string ler_html() {
    ifstream f("index.html");
    stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

int main() {
    vector<int> coins = {1, 5, 10, 25, 50, 100};
    

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Erro no Bind. A porta 8080 ja deve estar em uso." << endl;
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        cerr << "Erro no Listen." << endl;
        return 1;
    }

    cout << "=========================================" << endl;
    cout << "               Servidor :  "               << endl;
    cout << "         http://localhost:8080          "  << endl;
    cout << "=========================================" << endl;

    char buffer[30000] = {0};

    while (true) {
        int new_socket = accept(server_fd, nullptr, nullptr);
        if (new_socket < 0) continue;

        memset(buffer, 0, sizeof(buffer));
        read(new_socket, buffer, 30000);
        string request(buffer);

        if (request.find("GET /troco/") != string::npos) {
            size_t pos = request.find("/troco/");
            string sub = request.substr(pos + 7);
            size_t end_pos = sub.find(" ");
            int M = stoi(sub.substr(0, end_pos));


            auto resultado = coin_change(M, coins);

            string json_response = "{\"quantidade\":" + to_string(resultado.first) + ", \"usadas\":[";
            for (size_t i = 0; i < resultado.second.size(); ++i) {
                json_response += to_string(resultado.second[i]);
                if (i < resultado.second.size() - 1) json_response += ",";
            }
            json_response += "]}";

            string http_response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Length: " + to_string(json_response.length()) + "\r\n"
                "Connection: close\r\n\r\n" + json_response;

            write(new_socket, http_response.c_str(), http_response.length());
        } 
        else {
            string html_content = ler_html();
            string http_response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html; charset=utf-8\r\n"
                "Content-Length: " + to_string(html_content.length()) + "\r\n"
                "Connection: close\r\n\r\n" + html_content;

            write(new_socket, http_response.c_str(), http_response.length());
        }
        close(new_socket);
    }

    close(server_fd);
    return 0;
}