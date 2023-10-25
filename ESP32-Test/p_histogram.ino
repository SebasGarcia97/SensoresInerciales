#define HISTO_INF 0
#define HISTO_SUP 10000
#define HISTO_N_STEP 100

#define recv_list_len N_BATCH/6
#define packet_received(p) recv_list[p/6] |= 1<<p%6

char base[64] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
byte recv_list[recv_list_len];
int histogram[HISTO_N_STEP];
int histogram_higher;
int histogram_lower;
int error_recv;
int receiveNb;
int error_send;
double avg;

long batchStart =0;

unsigned long n_sent;

void init_histo() {
  for(int i=0;i<recv_list_len;i++) {
    recv_list[i] = 0;
  }
  for(int i=0;i<HISTO_N_STEP;i++) {
    histogram[i] = 0;
  }
  histogram_higher = 0;
  histogram_lower = 0;
  error_recv=0;
  error_send=0;
  receiveNb=0;
  avg=0;
}

int fill_histo(long value, int packet_nb) {
  int index = (value-HISTO_INF) * HISTO_N_STEP / (HISTO_SUP - HISTO_INF);
  if(index >= HISTO_N_STEP) {
    histogram_higher++;
  } else if(index < 0) {
    histogram_lower++;
  } else {
    histogram[index]++;
  }

  packet_received(packet_nb);
  receiveNb++;
  avg += value;
}

void print_histo() {
  Serial.println("----------");
  Serial.printf("Units :\tus");
  Serial.println();
  Serial.printf("Bounds :\t%d\t%d", HISTO_INF, HISTO_SUP);
  Serial.println();
  Serial.printf("Nb_values :\t%d", HISTO_N_STEP);
  Serial.println();
  for(int i=0;i<HISTO_N_STEP;i++) {
    Serial.printf("%d\t", histogram[i]);
  }
  Serial.printf("%d",histogram_higher);
  
  Serial.println();
  Serial.printf("Average :\t%d\t", receiveNb != 0 ? int(avg/receiveNb) : -1);
  Serial.println();
  Serial.printf("Received :\t%d", receiveNb);
  Serial.println();
  Serial.printf("Sent :\t%d", n_sent-error_send);
  Serial.println();
  for(int i=0;i<recv_list_len;i++) {
    Serial.printf("%c",base[recv_list[i]]);
  }
  Serial.println();
}
