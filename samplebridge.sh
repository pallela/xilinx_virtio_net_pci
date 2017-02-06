ovs-vsctl add-br br0
ovs-vsctl add-port br0 xvm1 -- set Interface xvm1 type=xiluiovs
ovs-vsctl show
