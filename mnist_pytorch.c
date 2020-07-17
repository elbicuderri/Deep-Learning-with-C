#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

void load_data(float *output, char *name, int size);

void print_image(float *image, int batch, int height, int width, int channel);

void split_image_label(float *image, float *label, float *input, int total_number, int size);

void normalization(float *output, float *input, int size);

void convolution(float*output, float*image, float*kernel, int batch, int in_channel, int out_channel, int image_Height, int image_Width, int kernel_Height, int kernel_Width, int pad_top, int pad_bottom, int pad_left, int pad_right, int stride_height, int stride_width);

void maxpooling(float*output, float*input, int batch, int channel, int input_height, int input_width, int kernel_height, int kernel_width, int pad_top, int pad_bottom, int pad_left, int pad_right, int stride_height, int stride_width);

void dense(float *output, float *input, float *weight, float *bias, int batch, int input_height, int input_width, int channel, int output_dim);

void relu(float*output, float*input, int batch, int dim);

void batchnormalization(float *output, float *input, float *gamma, float *beta, float *mean, float *variance, float epsilon, int batch, int channel, int input_height, int input_width);

float exponential_sum(float *input, int length, int start);

void softmax(float *output, float *input, int batch, int dim);


int main()
{
	int total_number = 10;
	int image_size = 784;
	int data_size = total_number * (image_size + 1);
	float epsilon = 0.001f;

	float *data = (float*)malloc(data_size * sizeof(float));
	load_data(data, "mnist_test_float.bin", data_size);

	float *kernel = (float*)malloc(125 * sizeof(float));
	load_data(kernel, "kernel_torch.bin", 125);

	printf("kernel: \n");
	print_image(kernel, 5, 1, 5, 5);

	float *gamma = (float*)malloc(5 * sizeof(float));
	load_data(gamma, "gamma_torch.bin", 5);

	printf("gamma: \n");
	print_image(gamma, 5, 1, 1, 1);

	float *beta = (float*)malloc(5 * sizeof(float));;
	load_data(beta, "beta_torch.bin", 5);

	printf("beta: \n");
	print_image(beta, 5, 1, 1, 1);

	float *mean = (float*)malloc(5 * sizeof(float));;
	load_data(mean, "mean_torch.bin", 5);

	printf("mean: \n");
	print_image(mean, 1, 1, 1, 5);

	float *variance = (float*)malloc(5 * sizeof(float));;
	load_data(variance, "variance_torch.bin", 5);

	printf("variance: \n");
	print_image(variance, 5, 1, 1, 1);

	float *W2 = (float*)malloc(980 * 120 * sizeof(float));;
	load_data(W2, "W2_torch.bin", 980 * 120);

	float *b2 = (float*)malloc(120 * sizeof(float));;
	load_data(b2, "b2_torch.bin", 120);

	float *W3 = (float*)malloc(120 * 10 * sizeof(float));;
	load_data(W3, "W3_torch.bin", 120 * 10);

	float *b3 = (float*)malloc(10 * sizeof(float));;
	load_data(b3, "b3_torch.bin", 10);

	printf("b3: \n");
	print_image(b3, 10, 1, 1, 1);

	//====================================================================

	//conv, maxpool, batchnorm, relu_batchnorm, dense1, relu_dense1, dense2, result

	//====================================================================

	float *image = (float*)malloc((total_number *  image_size) * sizeof(float));

	float *label = (float*)malloc(total_number * sizeof(float));

	split_image_label(image, label, data, total_number, image_size);

	//print_image(image, 1, 1, 28, 28);

	float *image_norm = (float*)malloc((total_number *  image_size) * sizeof(float));

	normalization(image_norm, image, total_number *  image_size);

	float *conv_origin = (float*)malloc((total_number * 28 * 28 * 5) * sizeof(float));
	load_data(conv_origin, "conv_torch.bin", total_number * 28 * 28 * 5);

	float *conv = (float*)malloc(total_number * 28 * 28 * 5 * sizeof(float));
	convolution(conv, image_norm, kernel, total_number, 1, 5, 28, 28, 5, 5, 2, 2, 2, 2, 1, 1);

	//printf("conv_origin: \n");
	//print_image(conv_origin, total_number, 5, 28, 28);

	//printf("conv : \n");
	//print_image(conv, total_number, 5, 28, 28);

	float *batchnorm_origin = (float*)malloc((total_number * 28 * 28 * 5) * sizeof(float));
	load_data(batchnorm_origin, "batchnorm_torch.bin", total_number * 28 * 28 * 5);

	float *batchnorm = (float*)malloc(total_number * 28 * 28 * 5 * sizeof(float));
	batchnormalization(batchnorm, conv, gamma, beta, mean, variance, epsilon, total_number, 5, 28, 28);

	//printf("batchnorm_origin: \n");
	//print_image(batchnorm_origin, total_number, 5, 28, 28);

	//printf("batchnorm: \n");
	//print_image(batchnorm, total_number, 5, 28, 28);

	float *maxpool_origin = (float*)malloc((total_number * 14 * 14 * 5) * sizeof(float));
	load_data(maxpool_origin, "maxpool_torch.bin", total_number * 14 * 14 * 5);

	float *maxpool = (float*)malloc(total_number * 14 * 14 * 5 * sizeof(float));
	maxpooling(maxpool, batchnorm, total_number, 5, 28, 28, 2, 2, 0, 0, 0, 0, 2, 2);

	//printf("maxpool_origin: \n");
	//print_image(maxpool_origin, total_number, 5, 14, 14);

	//printf("maxpool: \n");
	//print_image(maxpool, total_number, 5, 14, 14);

	float *relu_maxpool_origin = (float*)malloc((total_number * 14 * 14 * 5) * sizeof(float));
	load_data(relu_maxpool_origin, "relu_maxpool_torch.bin", total_number * 14 * 14 * 5);

	float *relu_maxpool = (float*)malloc(total_number * 14 * 14 * 5 * sizeof(float));
	relu(relu_maxpool, maxpool, total_number, 14 * 14 * 5);

	//printf("relu_maxpool_origin: \n");
	//print_image(relu_maxpool_origin, total_number, 5, 14, 14);

	//printf("relu_maxpool: \n");
	//print_image(relu_maxpool, total_number, 5, 14, 14);

	float *dense1_origin = (float*)malloc(total_number * 120 * sizeof(float));;
	load_data(dense1_origin, "dense1_torch.bin", total_number * 120);

	float *dense1 = (float*)malloc(total_number * 120 * sizeof(float));
	dense(dense1, relu_maxpool, W2, b2, total_number, 14, 14, 5, 120);

	//printf("dense1_origin: \n");
	//print_image(dense1_origin, total_number, 1, 12, 10);

	//printf("dense1: \n");
	//print_image(dense1, total_number, 1, 12, 10);

	float *relu_dense1_origin = (float*)malloc(total_number * 120 * sizeof(float));
	load_data(relu_dense1_origin, "relu_dense1_torch.bin", total_number * 120);

	float *relu_dense1 = (float*)malloc(total_number * 120 * sizeof(float));
	relu(relu_dense1, dense1, total_number, 120);

	//printf("relu_dense1_origin: \n");
	//print_image(relu_dense1_origin, total_number, 1, 12, 10);

	//printf("relu_dense1: \n");
	//print_image(relu_dense1, total_number, 1,  12, 10);

	float *dense2_origin = (float*)malloc(total_number * 10 * sizeof(float));
	load_data(dense2_origin, "dense2_torch.bin", total_number * 10);

	float *dense2 = (float*)malloc(total_number * 120 * 10 * sizeof(float));
	dense(dense2, relu_dense1, W3, b3, total_number, 1, 120, 1, 10);

	//printf("before softmax origin: \n");
	//print_image(dense2_origin, total_number, 1, 10, 1);

	//printf("before softmax : \n");
	//print_image(dense2, total_number, 1, 10, 1);

	float *result_origin = (float*)malloc(total_number * 10 * sizeof(float));
	load_data(result_origin, "result_torch.bin", total_number * 10);

	float *result = (float*)malloc(total_number * 10 * sizeof(float));
	softmax(result, dense2, total_number, 10);

	//printf("after softmax origin : \n");
	//print_image(result_origin, total_number, 1, 10, 1);

	//printf("after softmax : \n");
	//print_image(result , total_number, 1, 10, 1);

	printf("before softmax : \n\n\n");

	for (int n = 0; n < total_number; n++) {
		printf("%dth image result: \n\n", n + 1);
		for (int i = 0; i < 10; i++) {
			int index = n * 10 + i;
			float diff = dense2_origin[index] - dense2[index];
			printf("answer: %.8f, my answer: %.8f, difference: %.8f\n\n", dense2_origin[index], dense2[index], diff);
		}
		printf("\n");
	}

	printf("===============================================================================================================================================================================================================================\n\n");

	printf("after softmax : \n\n\n");

	for (int n = 0; n < total_number; n++) {
		printf("%dth image result: \n\n", n + 1);
		for (int i = 0; i < 10; i++) {
			int index = n * 10 + i;
			float diff = fabsf(result_origin[index] - result[index]);
			printf("answer: %.8f, my answer: %.8f, difference: %.8f\n\n", result_origin[index], result[index], diff);
		}
		printf("\n\n");
	}

	free(data);
	free(kernel);
	free(gamma);
	free(beta);
	free(mean);
	free(variance);
	free(W2);
	free(b2);
	free(W3);
	free(b3);
	free(image);
	free(label);
	free(image_norm);
	//free(image_transpose);
	free(conv_origin);
	free(conv);
	free(batchnorm_origin);
	free(batchnorm);
	free(maxpool_origin);
	free(maxpool);
	free(relu_maxpool_origin);
	free(relu_maxpool);
	free(dense1_origin);
	free(dense1);
	free(relu_dense1_origin);
	free(relu_dense1);
	free(dense2_origin);
	free(dense2);
	free(result_origin);
	free(result);

	return 0;

}

void load_data(float *output, char *name, int size)
{
	FILE *pFile = fopen(name, "rb");

	if (pFile == NULL) {
		printf("cannot find file\n");
		exit(-1);
	}

	size_t sizet = fread(output, size * sizeof(float), 1, pFile);

	if (sizet != 1) {
		printf("read error!\n");
		exit(-1);
	}

	fclose(pFile);
}

void print_image(float *image, int batch, int channel, int height, int width)
{
	int N = batch;
	int C = channel;
	int H = height;
	int W = width;

	for (int n = 0; n < N; n++) {
		for (int c = 0; c < C; c++) {
			for (int h = 0; h < height; h++) {
				for (int w = 0; w < width; w++) {
					printf("%6.2f ", image[n * C * H * W + c * H * W + h * W + w]);
				}
				printf("\n\n");
			}
			printf("\n===============================================================================================================================================================================================================================\n");
		}
	}

}

void split_image_label(float *image, float *label, float *input, int total_number, int size)
{
	int N = total_number;
	int S = size;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < S; j++) {
			int index = i * S + j;
			image[index] = input[i * (S + 1) + (j + 1)];
		}
	}

	for (int i = 0; i < N; i++) {
		label[i] = input[i * (S + 1)];
	}
}

void normalization(float *output, float *input, int size)
{
	float m = 255.0;
	for (int i = 0; i < size; i++) {
		output[i] = input[i] / m;
	}
}

void convolution(float *output, float *input, float *kernel, int batch, int in_channel, int out_channel, int input_height, int input_width, int kernel_height, int kernel_width, int pad_top, int pad_bottom, int pad_left, int pad_right, int stride_height, int stride_width)
{
	int N = batch;
	int C = in_channel;
	int K = out_channel;
	int H = input_height;
	int W = input_width;
	int kH = kernel_height;
	int kW = kernel_width;
	int pT = pad_top;
	int pB = pad_bottom;
	int pL = pad_left;
	int pR = pad_right;
	int sH = stride_height;
	int sW = stride_width;
	int pH = H + pT + pB;
	int pW = W + pL + pR;
	int oH = ((input_height + pad_top + pad_bottom - kernel_height) / stride_height) + 1; // output_height
	int oW = ((input_width + pad_left + pad_right - kernel_width) / stride_width) + 1; // output_width

	//pad image
	float *pad_image;
	pad_image = (float*)malloc((N * C * pH * pW) * sizeof(float));

	//fill pad image
	for (int n = 0; n < N; n++) {
		for (int c = 0; c < C; c++) {
			for (int ph = 0; ph < pH; ph++) {
				for (int pw = 0; pw < pW; pw++) {
					int pad_index = n * C * pH * pW + c * pH * pW + ph * pW + pw;
					if (ph < pT || ph >= H + pT || pw < pL || pw >= W + pL) {
						pad_image[pad_index] = 0;
					}
					else {
						int input_index = n * C * H * W + c * H * W + (ph - pT) * W + pw - pL;
						pad_image[pad_index] = input[input_index];
					}
				}
			}
		}
	}

	//printf("pad_image during convolution: \n");
	//print_image(pad_image, N * C * pH, pW);

	//convolution
	for (int n = 0; n < N; n++) {
		for (int k = 0; k < K; k++) {
			for (int oh = 0; oh < oH; oh++) { //image_row
				for (int ow = 0; ow < oW; ow++) { //image_column
					float sum = 0;
					for (int c = 0; c < C; c++) {
						for (int kh = 0; kh < kH; kh++) { //kernel_height
							for (int kw = 0; kw < kW; kw++) { //kernel_width
								int kernel_index = k * C * kH * kW + c * kH * kW + kh * kW + kw;
								int pad_index = n * C * pH * pW + c * pH * pW + oh * sH * pW + ow * sW + (kh * pW + kw);
								float s = pad_image[pad_index] * kernel[kernel_index];
								sum += s;
							}
						}
					}
					int output_index = n * K * oH * oW + k * oH * oW + oh * oW + ow;
					output[output_index] = sum;
				}
			}
		}
	}

	//printf("output after convolution: \n");
	//print_image(output, N * K * oH, oW);

	free(pad_image);

}


void maxpooling(float *output, float *input, int batch, int channel, int input_height, int input_width, int kernel_height, int kernel_width, int pad_top, int pad_bottom, int pad_left, int pad_right, int stride_height, int stride_width)
{
	int N = batch;
	int C = channel;
	int H = input_height;
	int W = input_width;
	int kH = kernel_height;
	int kW = kernel_width;
	int pT = pad_top;
	int pB = pad_bottom;
	int pL = pad_left;
	int pR = pad_right;
	int sH = stride_height;
	int sW = stride_width;
	int pH = H + pT + pB;
	int pW = W + pL + pR;
	int oH = ((input_height + pad_top + pad_bottom - kernel_height) / stride_height) + 1;
	int oW = ((input_width + pad_left + pad_right - kernel_width) / stride_width) + 1;

	float *pad_input;
	pad_input = (float*)malloc((N  * C * pH * pW) * sizeof(float));

	//fill pad image
	for (int n = 0; n < N; n++) {
		for (int c = 0; c < C; c++) {
			for (int ph = 0; ph < pH; ph++) {
				for (int pw = 0; pw < pW; pw++) {
					int pad_index = n * C * pH * pW + c * pH * pW + ph * pW + pw;
					if (ph < pT || ph >= H + pT || pw < pL || pw >= W + pL) {
						pad_input[pad_index] = 0;
					}
					else {
						int input_index = n * C * H * W + c * H * W + (ph - pT) * W + pw - pL;
						pad_input[pad_index] = input[input_index];
					}
				}
			}
		}
	}

	//printf("pad_image durin maxpooling: \n");
	//print_image(pad_input, N , C,  pH, pW);

	//maxpooling
	for (int n = 0; n < N; n++) {
		for (int c = 0; c < C; c++) {
			for (int oh = 0; oh < oH; oh++) {
 				for (int ow = 0; ow < oW; ow++) {
					float max = -FLT_MAX;
					for (int kh = 0; kh < kH; kh++) {
						for (int kw = 0; kw < kW; kw++) {
							int pad_index = n * C * pH * pW + c * pH * pW + (oh * sH + kh) * pW + ow * sW + kw;
							if (pad_input[pad_index] > max) {
								max = pad_input[pad_index];
							}
						}
					}
					int output_index = n * C * oH * oW + c * oH * oW + oh * oW + ow;
					output[output_index] = max;
				}
			}
		}
	}

	//printf("output after maxpooling: \n");
	//print_image(output, N * C * oH, oW);

	free(pad_input);

}

void dense(float *output, float *input, float *weight, float *bias, int batch, int input_height, int input_width, int channel, int output_dim)
{
	int N = batch;
	int H = input_height;
	int W = input_width;
	int C = channel;
	int K = output_dim;

	for (int n = 0; n < N; n++) {
		for (int k = 0; k < K; k++) {
			float sum = 0;
			for (int c = 0; c < C; c++) {
				for (int h = 0; h < H; h++) {
					for (int w = 0; w < W; w++) {
						int input_index = n * C * H * W + c * H * W + h * W + w;
						int weight_index = k * C * H * W + c * H * W + h * W + w;
						float s = input[input_index] * weight[weight_index];
						sum += s;
					}
				}
			}
			sum += bias[k];
			int output_index = n * K + k;
			output[output_index] = sum;
		}
	}

}

void relu(float *output, float *input, int batch, int dim)
{
	int N = batch;
	int D = dim;

	for (int n = 0; n < N; n++) {
		for (int i = 0; i < D; i++) {
			int index = n * D + i;
			if (input[index] >= 0) {
				output[index] = input[index];
			}
			else if (input[index] < 0) {
				output[index] = 0;
			}
		}
	}

}

void batchnormalization(float *output, float *input, float *gamma, float *beta, float *mean, float *variance, float epsilon, int batch, int channel, int input_height, int input_width)
{
	int N = batch;
	int H = input_height;
	int W = input_width;
	int C = channel;

	for (int n = 0; n < N; n++) {
		for (int c = 0; c < C; c++) {
			for (int h = 0; h < H; h++) {
				for (int w = 0; w < W; w++) {
					float result = ((gamma[c] * (input[n * C * H * W + c * H * W + h * W + w] - mean[c])) / (sqrtf(variance[c] + epsilon))) + beta[c];
					output[n * C * H * W + c * H * W + h * W + w] = result;
				}
			}
		}
	}

}

float exponential_sum(float *input, int length, int start)
{
	int L = length;
	int S = start;

	float sum = 0;
	for (int i = 0; i < L; i++) {
		float element = input[i + S];
		float element_exponential = expf(element);
		sum += element_exponential;
	}

	return sum;

}

void softmax(float *output, float *input, int batch, int length)
{
	int N = batch;
	int L = length;

	for (int i = 0; i < N; i++) {
		float exp_sum = exponential_sum(input, L, i * L);
		for (int j = 0; j < L; j++) {
			int index = i * L + j;
			output[index] = expf(input[index]) / exp_sum;
		}
	}

}
