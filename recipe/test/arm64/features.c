#include <hdf5.h>
#include <H5FDros3.h>
#include <string.h>

int main(void) {
    unsigned short input[32], output[32];
    for (int i = 0; i < 32; ++i) input[i] = (unsigned short)(i * 17);
    if (H5Zfilter_avail(H5Z_FILTER_SZIP) <= 0) return 1;
    hsize_t dimensions[1] = {32};
    hid_t file = H5Fcreate("szip-roundtrip.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hid_t space = H5Screate_simple(1, dimensions, NULL);
    hid_t properties = H5Pcreate(H5P_DATASET_CREATE);
    if (file < 0 || space < 0 || properties < 0) return 2;
    if (H5Pset_chunk(properties, 1, dimensions) < 0 ||
        H5Pset_szip(properties, H5_SZIP_NN_OPTION_MASK, 8) < 0) return 3;
    hid_t dataset = H5Dcreate2(file, "samples", H5T_NATIVE_USHORT, space,
                             H5P_DEFAULT, properties, H5P_DEFAULT);
    if (dataset < 0 || H5Dwrite(dataset, H5T_NATIVE_USHORT, H5S_ALL, H5S_ALL,
                              H5P_DEFAULT, input) < 0) return 4;
    if (H5Dclose(dataset) < 0 || H5Fclose(file) < 0) return 5;
    file = H5Fopen("szip-roundtrip.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
    dataset = H5Dopen2(file, "samples", H5P_DEFAULT);
    if (file < 0 || dataset < 0 || H5Dread(dataset, H5T_NATIVE_USHORT,
        H5S_ALL, H5S_ALL, H5P_DEFAULT, output) < 0 ||
        memcmp(input, output, sizeof(input)) != 0) return 6;
    H5Dclose(dataset); H5Fclose(file); H5Sclose(space); H5Pclose(properties);
    H5FD_ros3_fapl_t config = {0};
    config.version = H5FD_CURR_ROS3_FAPL_T_VERSION;
    strcpy(config.aws_region, "us-east-1");
    hid_t access = H5Pcreate(H5P_FILE_ACCESS);
    if (access < 0 || H5Pset_fapl_ros3(access, &config) < 0) return 7;
    H5Pclose(access);
    return H5close() < 0 ? 8 : 0;
}
