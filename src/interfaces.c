#include "interfaces.h"
iface_desc create_interface_descriptor()
{
	iface_desc desc;
	desc.iface_typ = IFACE_BLUETOOTH;
	return desc;
}
