#pragma once

#include "common.h"

namespace StreamCompaction {
    namespace Thrust {
        StreamCompaction::Common::PerformanceTimer& timer();

        void scan(int n, int *odata, const int *idata, bool enableTimer = true);
        void sort(int n, int *odata, const int *idata, bool enableTimer = true);
    }
}
