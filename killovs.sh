ps -A | grep ovs-vswitchd | awk '{print $1}' | xargs  kill -9
ps -A | grep ovsdb-server | awk '{print $1}' | xargs  kill -9
