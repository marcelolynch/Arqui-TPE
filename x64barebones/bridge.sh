# Esto se llama despues de tap.sh; ya deben estar levantadas las interfaces TAP0 TAP1 TAP2
# Necesitan estar instalados: bridge-utils y uml-utilities

sudo ip link set dev br2 down
sudo brctl delbr br2
sudo brctl addbr br2
sudo brctl addif br2 tap0
sudo brctl addif br2 tap1
sudo brctl addif br2 tap2
sudo ip link set dev br2 up
sudo ip link set dev tap0 up
sudo ip link set dev tap1 up
sudo ip link set dev tap2 up
echo 1 > /proc/sys/net/ipv6/conf/br2/autoconf

