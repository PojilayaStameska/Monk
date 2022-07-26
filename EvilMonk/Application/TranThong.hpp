#ifndef TRAN_THONG_H
#define TRAN_THONG_H

#include <functional>

void TranThong(int xstart, int ystart, int xend, int yend, std::function<void(int, int)> callback);

#endif 
