__kernel void TransposeMatrix(
    __global float* matrixA,
    __global float* matrixC,
    int N
) {

    // Achar os índices globais
    int xGID = get_global_id(0); // coluna
    int yGID = get_global_id(1); // linha

    matrixC[xGID * N + yGID] = matrixA[yGID * N + xGID];
}
