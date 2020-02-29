kernel void add(global const float *a, global const float *b, global float *c, uint m, uint p) {
    uint id0 = get_global_id(0);
    uint id1 = get_global_id(1);

    float temp = 0;
    for (uint k = 0; k < m; k++) {
        temp += a[id0 * m + k] * b[id1 * m + k];
    }
    c[id0 * p + id1] = temp;
}
