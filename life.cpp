#include <iostream>
#include <vector>
#include <random>

#include <unistd.h>
#include <sys/time.h>

#define LIVING "■ "
#define DYING "  "

using namespace std;

class life {
        // dying, livingはそれぞれの状態で表示される記号
        const string dying = DYING, living = LIVING;
        const string cell[2] = {dying, living};

        int width, height;
        int livingRule, birthRule;
        vector<vector<string>> field;

        // フレームが切り替わる間隔(ナノ秒)
        timespec interframe = {0, 500 * 300000};            
public:
        /* 
         * 引数にフィールドの縦と横のマス数を取る。
         * 全てのセルの生死をランダムに初期化する。
         */
        life(int argWidth = 150, int argHeight = 80) {
                width = argWidth + 2;
                height = argHeight + 2;

                field.resize(height);
                for (int i = 0; i < height; i++) {
                        field[i].resize(width);
                }

                random_device rnd;
                mt19937 mt(rnd());
                uniform_int_distribution<> uniRnd(0, 1);
        
                for (int i = 0; i < height; i++) {
                        for (int j = 0; j < width; j++) {
                                field[i][j] = cell[uniRnd(mt)];
                        }
                }
        }

        /*
         * life gameをはじめる。
         * 無限リピートだからプロセスを強制終了して終わらせる。
         */
        void start() {
                string tmp[height][width];
                while(1) {
                        system("clear");
                        for (vector<string> i : field) {
                                for (string j : i) {
                                        cout << j;
                                }
                                cout << endl;
                        }

                        for (int i = 1; i < height - 1; i++) {
                                for (int j = 1; j < width - 1; j++) {
                                        bool cellStatus = true;
                                        if (field[i][j] == dying) cellStatus = false;

                                        int livingCount = 0;
                                        if (field[i - 1][j - 1] == living) livingCount++;
                                        if (field[i][j - 1] == living) livingCount++;
                                        if (field[i + 1][j - 1] == living) livingCount++;
                                        if (field[i - 1][j] == living) livingCount++;
                                        if (field[i + 1][j] == living) livingCount++;
                                        if (field[i - 1][j + 1] == living) livingCount++;
                                        if (field[i][j + 1] == living) livingCount++;
                                        if (field[i + 1][j + 1] == living) livingCount++;

                                        if (cellStatus) {
                                                if (livingCount == 2 || livingCount == 3) 
                                                        tmp[i][j] = living;
                                                else tmp[i][j] = dying;
                                        }
                                        else if (livingCount == 3 || livingCount == 6) tmp[i][j] = living;
                                        else tmp[i][j] = dying;
                                }
                        }

                        for (int i = 0; i < height; i++) {
                                for (int j = 0; j < width; j++) {
                                         field[i][j] = tmp[i][j];
                                }
                        }
                        nanosleep(&interframe, NULL);
                }
        }
};

int main()
{
            life obj;
            obj.start();
}
