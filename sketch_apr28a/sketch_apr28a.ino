#include <dnnrt/runtime.h>

const char bin_model_affine[] PROGMEM = {
#include "data/simple_model_affine.h"  
};

const char bin_model_relu[] PROGMEM = {
#include "data/simple_model_relu.h"  
};


const char bin_model_sigmoid[] PROGMEM = {
#include "data/simple_model_sigmoid.h"  
};

const char bin_model_tanh[] PROGMEM = {
#include "data/simple_model_tanh.h"  
};

const char bin_model_softmax[] PROGMEM = {
#include "data/simple_model_softmax.h"  
};

const float input_data[] = {-5,-2,0,4,6};

const char* models[] = {
  bin_model_affine,
  bin_model_relu,
  bin_model_sigmoid,
  bin_model_tanh,
  bin_model_softmax
};

nn_network_t *load_nnb_network(const char* bin_model)
{
  assert(bin_model != NULL);
  /* store content of nnb_file in heap memory */
  nn_network_t *network = (nn_network_t*)bin_model;
  printf("binary format version %d\n", network->version);
  printf("function-level compatibility revision %d\n", network->revision); 
  return network;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  /* initialize the whole dnnrt subsystem */
  const dnn_config_t config = {.cpu_num = 1 };
  int ret = dnn_initialize(&config);
  if (0 != ret) {
      printf("dnn_initialize() failed due to %d", ret);
      goto dnn_error;
  }
  printf("float size = %d", sizeof(float));
  printf("\n");
  printf("dnn_initialize() succeeded\n");

  for (int i = 0; i < sizeof(models) / sizeof(models[0]); i++) {
    printf("\n");
    printf("Run the model #%d", i);
    printf("\n");
    nn_network_t *network = load_nnb_network(models[i]);
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
    
    nn_variable_t *in_var = dnn_runtime_input_variable(&rt, 0);
    printf("var type = %d (NN_DATA_TYPE_FLOAT=%d)\n", in_var->type, NN_DATA_TYPE_FLOAT);
  
    const void *inputs[1] = { (const void *)input_data };
    ret = dnn_runtime_forward(&rt, inputs, 1);
    if (ret) {
        printf("dnn_runtime_forward() failed due to %d\n", ret);
        goto fin_error;
    }
    printf("dnn_runtime_forward() succeeded\n");
  
    float* output_buffer = dnn_runtime_output_buffer(&rt, 0);
    for (int i = 0; i < nbr_outputs; i++) {
      printf("[output %d] %.4f\n", i, output_buffer[i]);
    }
  
  fin_error:
    /* free memories allocated to dnn_runtime_t */
    dnn_runtime_finalize(&rt);
  }
rt_error:
  /* finalize the whole dnnrt subsystem */
  dnn_finalize();
dnn_error:
  printf("Application finalized\n");

}

void loop() {
  // put your main code here, to run repeatedly:

}
