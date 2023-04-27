template<typename T>
static inline T my_max(T val1, T val2) {
    if (val1 >= val2)
        return val1;
    return val2;
}

template<typename T>
static inline T my_min(T val1, T val2) {
    if (val1 <= val2)
        return val1;
    return val2;
}

template<typename T>
static inline T _clamp(T val, T minVal, T maxVal) {
    return my_min(my_max(val, minVal), maxVal);
}
