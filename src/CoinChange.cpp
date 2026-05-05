#include <bits/stdc++.h>
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

int main() {
    vector<int> coins = {1, 5, 10, 25, 50, 100};

    int M;
    cout << "Digite o valor do troco: ";
    cin >> M;

    auto resultado = coin_change(M, coins);

    cout << "Numero de moedas: " << resultado.first << "\n";
    cout << "Moedas usadas: ";

    for (int c : resultado.second)
        cout << c << " ";

    cout << endl;
    return 0;
}
