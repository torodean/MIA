#!/bin/bash

# This script is in charge of performing install options beyong what is done during the
# cmake build. This can include adding configuration files or changing config values.

sudo mkdir -p /var/log/mia
sudo touch /var/log/mia/MIA.log
sudo chmod a+rw /var/log/mia/MIA.log
