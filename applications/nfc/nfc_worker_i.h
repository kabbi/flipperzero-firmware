#pragma once

#include "nfc_types.h"
#include "nfc_worker.h"

#include <furi.h>
#include <stdbool.h>

#include <rfal_analogConfig.h>
#include <rfal_rf.h>
#include <rfal_nfc.h>
#include <rfal_nfca.h>
#include <rfal_nfcb.h>
#include <rfal_nfcf.h>
#include <rfal_nfcv.h>
#include <st25r3916.h>
#include <st25r3916_irq.h>

struct NfcWorker {
    osMessageQueueId_t message_queue;
    osThreadAttr_t thread_attr;
    osThreadId_t thread;

    NfcWorkerState state;
    ReturnCode error;
};

void nfc_worker_change_state(NfcWorker* nfc_worker, NfcWorkerState state);

void nfc_worker_task(void* context);

void nfc_worker_poll(NfcWorker* nfc_worker, uint8_t cycles);
bool nfc_worker_nfca_poll(NfcWorker* nfc_worker);
bool nfc_worker_nfcb_poll(NfcWorker* nfc_worker);
bool nfc_worker_nfcf_poll(NfcWorker* nfc_worker);
bool nfc_worker_nfcv_poll(NfcWorker* nfc_worker);

void nfc_worker_emulate(NfcWorker* nfc_worker);

void nfc_worker_field(NfcWorker* nfc_worker);