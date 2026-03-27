#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

int N, K;
vector<string> ciphers;

// Генерация всех слов длины K и их шифров
void generateWords(int pos, string currentWord, string currentCipher, 
                   map<string, vector<string>>& cipherToWords) {
    if (pos == K) {
        cipherToWords[currentCipher].push_back(currentWord);
        return;
    }
    
    for (int i = 0; i < N; i++) {
        generateWords(pos + 1, 
                      currentWord + char('a' + i), 
                      currentCipher + ciphers[i] + " ", 
                      cipherToWords);
    }
}

int main() {
    cout << "========================================" << endl;
    cout << "PROGRAMMA DLYA OPREDELENIYA ODNOSVYAZNYH SHIFROV" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    cout << "Vvedite N (kolichestvo simvolov v alfavite) i K (dlinu slova): ";
    cin >> N >> K;
    cout << "N = " << N << ", K = " << K << endl;
    cout << endl;
    
    ciphers.resize(N);
    cout << "Vvedite " << N << " shifrov dlya simvolov ot 'a' do '" << char('a' + N - 1) << "':" << endl;
    for (int i = 0; i < N; i++) {
        cout << "  Shifr dlya simvola '" << char('a' + i) << "': ";
        cin >> ciphers[i];
    }
    
    cout << endl;
    cout << "Vvedennye shifry:" << endl;
    for (int i = 0; i < N; i++) {
        cout << "  " << char('a' + i) << " -> " << ciphers[i] << endl;
    }
    cout << endl;
    
    cout << "Generaciya vseh vozmozhnyh slov dliny " << K << "..." << endl;
    
    // Словарь: шифр -> список слов с таким шифром
    map<string, vector<string>> cipherToWords;
    
    // Генерируем все возможные слова длины K
    generateWords(0, "", "", cipherToWords);
    
    cout << "Vsego sgenerirovano slov: " << cipherToWords.size() << endl;
    cout << endl;
    
    // Считаем слова, которые можно однозначно расшифровать
    int uniqueCount = 0;
    cout << "Rezultaty:" << endl;
    cout << "----------------------------------------" << endl;
    for (auto& pair : cipherToWords) {
        cout << "Shifr: \"" << pair.first << "\" -> ";
        for (size_t i = 0; i < pair.second.size(); i++) {
            cout << pair.second[i];
            if (i < pair.second.size() - 1) cout << ", ";
        }
        if (pair.second.size() == 1) {
            cout << " (ODNOZNAChNO)";
            uniqueCount++;
        }
        cout << endl;
    }
    cout << "----------------------------------------" << endl;
    cout << endl;
    
    cout << "Kolichestvo slov, kotorye mozhno odnoznachno rasshifrovat: " << uniqueCount << endl;
    cout << "========================================" << endl;
    
    return 0;
}
