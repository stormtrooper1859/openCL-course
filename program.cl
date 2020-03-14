kernel void add(global const float *a, global const float *b, global float *c, const uint m, const uint p) {
    uint id0 = get_global_id(0);
    uint id1 = get_global_id(1);

    uint x = get_local_id(0);
    uint y = get_local_id(1);

//    uint ls0 = get_local_size(0);
//    uint ls1 = get_local_size(1);

    uint cache0 = id0 * m + y;
    uint cache1 = id1 * m + x;
    float temp = 0;

//    local float at[LS1 * LS1];
//    local float bt[LS1 * LS1];
    local float at[LS1][LS1];
    local float bt[LS1][LS1];

    uint title_count = m / LS1;
    for (uint i = 0; i < title_count; i++) {
//        at[x * LS1 + y] = a[cache0 + i * LS1 + y];
//        bt[y * LS1 + x] = b[cache1 + i * LS1 + x];
//        at[x][y] = a[cache0 + i * LS1];
//        bt[x][y] = b[cache1 + i * LS1];
        at[y][x] = a[cache0 + i * LS1];
        bt[y][x] = b[cache1 + i * LS1];

        barrier(CLK_LOCAL_MEM_FENCE);

        for (uint k = 0; k < LS1; k++) {
//            temp += at[x * LS1 + k] * bt[y * LS1 + k];
//            temp += at[x][k] * bt[k][y];
            temp += at[k][x] * bt[y][k];
        }

        barrier(CLK_LOCAL_MEM_FENCE);
    }
    c[id0 * p + id1] = temp;
}
