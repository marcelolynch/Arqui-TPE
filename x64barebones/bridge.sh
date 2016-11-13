sudo tunctl -u $(whoami)
sudo tunctl -u $(whoami)
sudo tunctl -u $(whoami)
sudo brctl addbr br2
sudo brctl addif br2 tap3
sudo brctl addif br2 tap4
sudo brctl addif br2 tap5
sudo ip link set dev br2 up
sudo ip link set dev tap0 up
sudo ip link set dev tap1 up
sudo ip link set dev tap2 up