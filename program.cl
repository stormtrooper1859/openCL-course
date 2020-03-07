#define BUTCH_SIZE 64
#define LS0 8
#define LS1 8

kernel void add(global const float *a, global const float *b, global float *c, const uint m, const uint p) {
    uint id0 = get_global_id(0);
    uint id1 = get_global_id(1);

    uint lid0 = get_local_id(0);
    uint lid1 = get_local_id(1);

    uint ls0 = get_local_size(0);
    uint ls1 = get_local_size(1);

    uint cache1 = id0 * m;
    uint cache2 = id1 * m;
    float temp = 0;

    local float at[LS0 * LS1];
    local float bt[LS0 * LS1];

    for (uint i = 0; i < m / LS1; i++) {
        at[ls0 * LS0 + ls1] = a[cache1 + i * LS1 + ls0];
        bt[ls1 * LS0 + ls0] = b[cache2 + i * LS1 + ls1];

        barrier(CLK_LOCAL_MEM_FENCE);

        for (uint k = 0; k < LS1; k++) {
            temp += at[ls0 * LS1 + k] * bt[ls1 * LS1 + k];
        }
    }
    c[id0 * p + id1] = temp;
}
