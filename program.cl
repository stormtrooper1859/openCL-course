kernel void add(global const float *a, global const float *b, global float *c, const uint m, const uint p) {
    uint id0 = get_global_id(0);
    uint id1 = get_global_id(1);

    uint cache1 = id0 * m;
    uint cache2 = id1 * m;

    local float ln1[256];
    local float ln2[256];
    local float ln3[256];
    local float ln4[256];



    barrier(CLK_LOCAL_MEM_FENCE);
    float temp = 0;
    for (uint k = 0; k < m; k++) {
        temp += a[cache1 + k] * b[cache2 + k];
    }
    c[id0 * p + id1] = temp;
}
