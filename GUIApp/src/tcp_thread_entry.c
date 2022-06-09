#include "tcp_thread.h"
#include "dhcp/dhcp_setup.h"
#include "tcp/MessageBody.h"
#include "neural_network/Lista.h"
#include "neural_network/ECG.h"
#include "neural_network/NeuralNetwork.h"
#include "neural_network/Layer.h"
#include "guiapp_event_handlers.h"
#include <math.h>

static FX_FILE g_file;
char fileContent[6000];

NeuralNetwork neural_network = NULL;
Layer input_layer, hidden_layer, output_layer = NULL;

Lista loadECGFile(int file){
    UINT status;
    ULONG actual_bytes;
    char fileName[20];
    sprintf(fileName, "%d.json", file);
    status = fx_file_open(g_fx_media_ptr, &g_file, fileName, FX_OPEN_FOR_READ);
    if (FX_SUCCESS != status)
    {
        __BKPT(0);
    }

    fx_file_read(&g_file, fileContent, 4096, &actual_bytes);
    Lista list = JsonArrayToECG(fileContent);
    status = fx_file_close(&g_file);
    if (FX_SUCCESS != status)
    {
        __BKPT(0);
    }

    status = fx_media_flush(g_fx_media_ptr);
    if (FX_SUCCESS != status)
    {
        __BKPT(0);
    }

    return list;
}

NX_TCP_SOCKET socket_echo;

char data[512];

/* TCP Thread entry function */
void tcp_thread_entry(void)
{
   UINT error_counter = 0;
   UINT status;
   ULONG bytes_copied;
   NX_PACKET       *packet_ptr;

   /* Wait for the IP stack and network hardware
    * to get initialized.
    */
   tx_thread_sleep(3 *NX_IP_PERIODIC_RATE);

   /* Start and run a brief DHCP Client session. */
   status = run_dhcp_client_session(&g_dhcp_client0, &g_ip0);

   /* Check for successful result. */
   if (status)
       error_counter++;

   if(neural_network == NULL){
       neural_network = createNeuralNetwork();
       input_layer = createLayer(NULL,8,INPUT);
       addLayer(neural_network, input_layer);
       hidden_layer = createLayer("relu",4,HIDDEN);
       addLayer(neural_network, hidden_layer);
       output_layer = createLayer("sigmoid",1,OUTPUT);
       addLayer(neural_network, output_layer);
   }

/* Create a socket.  */
  status =  nx_tcp_socket_create(&g_ip0, &socket_echo, "Echo Server Socket",
                              NX_IP_NORMAL, NX_FRAGMENT_OKAY, NX_IP_TIME_TO_LIVE, 512,
                              NX_NULL, NX_NULL);

  /* Check for error.  */
  if (status)
      error_counter++;

  /* Setup this thread to listen.  */
  status =  nx_tcp_server_socket_listen(&g_ip0, 3333, &socket_echo, 5,NX_NULL);

  /* Check for error.  */
  if (status)
      error_counter++;

  /* Loop to create and establish server connections.  */
  while(1)
  {

      /* Accept a client socket connection.  */
      status =  nx_tcp_server_socket_accept(&socket_echo, NX_WAIT_FOREVER);

      /* Check for error.  */
      if (status)
          error_counter++;

      setConnection();

      while(1){
          /* Receive a TCP message from the socket.  */
          status =  nx_tcp_socket_receive(&socket_echo, &packet_ptr, NX_WAIT_FOREVER);

          nx_packet_data_extract_offset(packet_ptr, 0, &data, 512, &bytes_copied);

          MessageBody message = JsonToMessageBody(data);

          char * response;

          int ECGFileName;

          Lista  list = NULL;

          int A = 0, N = 0, idMsg, conn = 1;

          double nn_result;

          switch (getOpCode(message)) {
            case TEST_OP:
                idMsg = getIdMsg(message);
                free(message);
                message = createMessageBody();
                setOpCode(message, RESPONSE_TEST_OP);
                setIdMsg(message, idMsg+1);

                response = MessageBodyToJson(message);

                nx_packet_allocate(&g_packet_pool0, &packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);
                nx_packet_data_append(packet_ptr, response, strlen(response), &g_packet_pool0, NX_WAIT_FOREVER);
                nx_tcp_socket_send(&socket_echo, packet_ptr, NX_WAIT_FOREVER);

                free(message);
                free(response);
                break;
            case FILE_EVALUATION:
                ECGFileName = getECGFile(message);
                idMsg = getIdMsg(message);
                free(message);
                message = createMessageBody();
                setOpCode(message, RESPONSE_FILE_EVALUATION);
                setECGFile(message, ECGFileName);
                setIdMsg(message, idMsg+1);

                list = loadECGFile(ECGFileName);
                int list_size = getListaSize(list);
                double hr_sum = 0;
                if(neural_network != NULL && list != NULL){
                    while(list != NULL){
                        hr_sum += 60 / getRRValue(get(list));
                        nn_result = executeNeuralNetwork(neural_network, get(list));
                        nn_result>0.5 ? A++ : N++;
                        list = getProx(list);
                    }
                    setHeartRate(message, round(hr_sum / list_size));
                    setGoodComplex(message, N);
                    setBadComplex(message, A);
                }
                response = MessageBodyToJson(message);
                nx_packet_allocate(&g_packet_pool0, &packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);
                nx_packet_data_append(packet_ptr, response, strlen(response), &g_packet_pool0, NX_WAIT_FOREVER);
                nx_tcp_socket_send(&socket_echo, packet_ptr, NX_WAIT_FOREVER);

                free(message);
                free(response);
                break;
            case GET_FILES:
                idMsg = getIdMsg(message);
                free(message);
                response = getECGFiles(idMsg);
                nx_packet_allocate(&g_packet_pool0, &packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);
                nx_packet_data_append(packet_ptr, response, strlen(response), &g_packet_pool0, NX_WAIT_FOREVER);
                nx_tcp_socket_send(&socket_echo, packet_ptr, NX_WAIT_FOREVER);
                free(response);
                break;
            case DISCONNECT:
                free(message);
                nx_tcp_socket_disconnect(&socket_echo, NX_WAIT_FOREVER);
                nx_tcp_server_socket_unaccept(&socket_echo);
                nx_tcp_server_socket_relisten(&g_ip0, 3333, &socket_echo);
                setWaitConn();
                conn = 0;
                break;
            default:
                idMsg = getIdMsg(message);
                free(message);
                message = createMessageBody();
                setOpCode(message, NOT_FOUND);
                setIdMsg(message, idMsg+1);
                response = MessageBodyToJson(message);
                nx_packet_allocate(&g_packet_pool0, &packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);
                nx_packet_data_append(packet_ptr, response, strlen(response), &g_packet_pool0, NX_WAIT_FOREVER);
                nx_tcp_socket_send(&socket_echo, packet_ptr, NX_WAIT_FOREVER);
                free(response);
                free(message);
                break;
          }

          if(!conn) break;
      }
  }

   /* Application has finished */
   while(1);
}
