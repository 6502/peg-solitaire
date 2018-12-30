#include <stdio.h>
#include <vector>
#include <string>

int main(int argc, const char *argv[]) {

    std::vector<std::string> board = {"##ooo##",
                                      "##ooo##",
                                      "ooooooo",
                                      "ooo ooo",
                                      "ooooooo",
                                      "##ooo##",
                                      "##ooo##"};
    int W = board[0].size();
    int H = board.size();

    typedef unsigned long long u64;

    std::vector<std::vector<u64>> bit(H, std::vector<u64>(W));

    int nb = 0;
    u64 start = 0;
    for (int i=0; i<H; i++) {
        for (int j=0; j<W; j++) {
            if (board[i][j] == 'o' || board[i][j] == ' ') {
                bit[i][j] = 1ull << nb;
                if (board[i][j] == 'o') {
                    start |= 1ull << nb;
                }
                nb += 1;
            }
        }
    }
    u64 goal = ((1ull<<nb)-1) ^ start;
    printf("%i bits\n", nb);

    auto hole = [&](u64 pos, int i, int j)->bool {
                    u64 m = bit[i][j];
                    return m != 0 && (pos & m) == 0;
                };

    auto peg = [&](u64 pos, int i, int j)->bool {
                   u64 m = bit[i][j];
                   return (pos & m) != 0;
               };

    auto play = [&](u64 pos, int i1, int j1, int i2, int j2, int i3, int j3)->u64 {
                    return pos ^ bit[i1][j1] ^ bit[i2][j2] ^ bit[i3][j3];
                };

    auto moves = [&](u64 pos)->std::vector<u64> {
        std::vector<u64> res;
        for (int i=0; i<H; i++) {
            for (int j=0; j<W; j++) {
                if (peg(pos, i, j)) {
                    if (i>1 && peg(pos, i-1, j) && hole(pos, i-2, j)) {
                        res.push_back(play(pos, i, j, i-1, j, i-2, j));
                    }
                    if (j>1 && peg(pos, i, j-1) && hole(pos, i, j-2)) {
                        res.push_back(play(pos, i, j, i, j-1, i, j-2));
                    }
                    if (i<H-2 && peg(pos, i+1, j) && hole(pos, i+2, j)) {
                        res.push_back(play(pos, i, j, i+1, j, i+2, j));
                    }
                    if (j<W-2 && peg(pos, i, j+1) && hole(pos, i, j+2)) {
                        res.push_back(play(pos, i, j, i, j+1, i, j+2));
                    }
                }
            }
        }
        return res;
    };

    std::vector<u64> active{start};
    int sz = ((1ull<<nb)+31) / 32;
    std::vector<u64> seen(sz);
    seen[start>>5] |= 1u<<(start&31);
    u64 count = 1;

    int step = 0;
    while (active.size()) {
        ++step;
        printf("%i %llu\n", step, count);
        std::vector<u64> na;
        for (u64 pos : active) {
            for (u64 nb : moves(pos)) {
                if ((seen[nb>>5] & (1u<<(nb&31))) == 0) {
                    seen[nb>>5] |= (1u<<(nb&31));
                    count++;
                    na.push_back(nb);
                    if (nb == goal) {
                        printf("Solved!\n");
                        std::vector<u64> solution;
                        for(;;) {
                            solution.push_back(nb);
                            u64 from = 0;
                            auto check = [&](u64 p) {
                                             if (seen[p>>5] & (1ull<<(p&31))) from = p;
                                         };
                            for (int i=0; i<H && !from; i++) {
                                for (int j=0; j<W && !from; j++) {
                                    if (peg(nb, i, j)) {
                                        if (i>1 && hole(nb, i-1, j) && hole(nb, i-2, j)) check(play(nb, i, j, i-1, j, i-2, j));
                                        if (j>1 && hole(nb, i, j-1) && hole(nb, i, j-2)) check(play(nb, i, j, i, j-1, i, j-2));
                                        if (i<H-2 && hole(nb, i+1, j) && hole(nb, i+2, j)) check(play(nb, i, j, i+1, j, i+2, j));
                                        if (j<W-2 && hole(nb, i, j+1) && hole(nb, i, j+2)) check(play(nb, i, j, i, j+1, i, j+2));
                                    }
                                }
                            }
                            if (!from) break;
                            nb = from;
                        }
                        while (solution.size()) {
                            u64 p = solution.back();
                            solution.pop_back();
                            for (int i=0; i<H; i++){
                                for (int j=0; j<W; j++) {
                                    if (peg(p, i, j)) putchar('o');
                                    else if (hole(p, i, j)) putchar('.');
                                    else putchar(' ');
                                }
                                putchar('\n');
                            }
                            putchar('\n');
                        }
                        exit(1);
                    }
                }
            }
        }
        active.swap(na);
    }
    printf("No solution!\n");
    return 0;
}
