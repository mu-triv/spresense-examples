#include <dnnrt/runtime.h>

const char bin_model[] PROGMEM = {
#include "data/cnn_static_graph_nnb.h"  
};

const char bin_infdata[] PROGMEM = {
#include "data/img_64010808.h"  
};

nn_network_t *alloc_nnb_network(const char* bin_model, uint32_t model_size)
{
  assert(bin_model != NULL);
  assert(model_size > 0) ;
  /* store content of nnb_file in heap memory */
  nn_network_t *network = malloc(model_size);
  if (network != NULL) {  
    memcpy(network, bin_model, model_size);
    return network;
  }
  return NULL;
}



void setup() {

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  nn_network_t *network = alloc_nnb_network(bin_model, sizeof(bin_model));
  
  /* initialize the whole dnnrt subsystem */
  const dnn_config_t config = {.cpu_num = 1 };
  int ret = dnn_initialize(&config);
  if (0 != ret) {
      printf("dnn_initialize() failed due to %d", ret);
      goto dnn_error;
  }
  printf("dnn_initialize() succeeded\n");

  /* instantiate a neural network defined by nn_network_t as a dnn_runtime_t object */
  dnn_runtime_t rt;
  ret = dnn_runtime_initialize(&rt, network);
  if (ret)  {
      printf("dnn_runtime_initialize() failed due to %d\n", ret);
      goto rt_error;
    }
  printf("dnn_runtime_initialize() succeeded\n");

  int nbr_inputs = dnn_runtime_input_num(&rt);
  int nbr_outputs = dnn_runtime_output_num(&rt);
  printf("nbr_inputs=%d nbr_outputs=%d\n", nbr_inputs, nbr_outputs);
  
  const int batch_size = 64;
  
  nn_variable_t *in_var = dnn_runtime_input_variable(&rt, 0);
  printf("var type = %d (NN_DATA_TYPE_FLOAT=%d)\n", in_var->type, NN_DATA_TYPE_FLOAT);
  printf("float size = %d", sizeof(float));
  printf("\n");

  const void *inputs[1] = { bin_infdata };
  ret = dnn_runtime_forward(&rt, inputs, 1);
  if (ret) {
      printf("dnn_runtime_forward() failed due to %d\n", ret);
      goto fin_error;
  }
  printf("dnn_runtime_forward() succeeded\n");

  /* Step-D: obtain the output from this dnn_runtime_t */
  float* output_buffer = dnn_runtime_output_buffer(&rt, 0u);
  for (int i = 0; i < batch_size; i++) {
    printf("[%d]: ", i);
    auto max_val = -1e-10;
    int number = 0;
    for (int j = 0; j < 10; j++) {
      auto val = output_buffer[i * 10 + j];
      printf("%.4f", val);
      printf(" ");
      if (val >= max_val) {
        max_val = val;
        number = j;
      }
    }
    printf("%d", number);
    printf("\n");
  }

fin_error:
  /* free memories allocated to dnn_runtime_t */
  dnn_runtime_finalize(&rt);
rt_error:
  /* finalize the whole dnnrt subsystem */
  dnn_finalize();
dnn_error:
  /* free model memory */
  free(network);
  printf("Application finalized\n");
}

void loop() {
  // put your main code here, to run repeatedly:

}
