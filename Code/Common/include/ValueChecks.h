#pragma once

namespace ValueChecks {

template <typename T>
static auto HasSingleBitSet(const T& val) -> bool {
    if (val == 0 || (val & (val - 1)) != 0) {
        return false;
    }

    return true;
}

}  // namespace ValueChecks