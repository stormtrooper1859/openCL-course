kernel void add(global const int *a, global const int *b, global int *c) {
    uint id = get_global_id(0);
    c[id] = a[id] + b[id];
}
