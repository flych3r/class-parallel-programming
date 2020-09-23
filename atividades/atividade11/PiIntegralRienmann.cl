__kernel void PiIntegralRienmann(
    __global double* a,
    __global double* c,
    __local double* ProductsWG,
    int iNumElements
) {
    // Recuperar o índice global
    int iGID = get_global_id(0);

    // Recuperar o índice local
    int iLID = get_local_id(0);

    // Recuperar o índice do grupo
    int iWGID = get_group_id(0);

    // Recuperar o tamanho do grupo
    int iWGS = get_local_size(0);

    double x, fx, temp = 0.0, dx = 1.0 / (iNumElements - 1);
    while (iGID < iNumElements) {
        x =  a[iGID] * dx;
        fx = sqrt(1.0 - x * x);
        temp += fx * dx;

        iGID += get_global_size(0);
    }

    // Armazenar o produto
    ProductsWG[iLID] = temp;

    // Esperar por todos os threads no grupo
    barrier(CLK_LOCAL_MEM_FENCE);

    // Redução
    int i = iWGS / 2;
    while (i != 0) {
        if (iLID < i) {
            ProductsWG[iLID] += ProductsWG[iLID+i];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
        i = i / 2;
    }

    // Armazenar os resultados parciais do produto na memória global
    if (iLID == 0) {
        c[iWGID] = ProductsWG[0];
    }
}
