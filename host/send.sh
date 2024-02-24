#!/bin/bash

# Path to the device file
DATA_FILE="../data"
DEVICE_FILE="../fifo"

# Simulate data writing to the device file
while true; do
    echo "Simulated data $(date)" > "${DATA_FILE}"
    cat "${DATA_FILE}" > "${DEVICE_FILE}"
    sleep 5
done
