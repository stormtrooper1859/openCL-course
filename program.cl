kernel void add(global const float *a, global const float *b, global float *c, const uint m, const uint p) {
    uint id0 = get_global_id(0);
    uint id1 = get_global_id(1);

    uint x = get_local_id(0);
    uint y = get_local_id(1);

//    uint ls0 = get_local_size(0);
//    uint ls1 = get_local_size(1);

    uint cache0 = id0 * m;
    uint cache1 = id1 * m;
    float temp = 0;

    local float at[LS0 * LS1];
    local float bt[LS0 * LS1];

    for (uint i = 0; i < (m / LS1); i++) {
        at[x * LS0 + y] = a[cache0 + i * LS0 + y];
        bt[y * LS1 + x] = b[cache1 + i * LS1 + x];

        barrier(CLK_LOCAL_MEM_FENCE);

        for (uint k = 0; k < LS1; k++) {
            temp += at[x * LS0 + k] * bt[y * LS1 + k];
        }
    }
    c[id0 * p + id1] = temp;
}
