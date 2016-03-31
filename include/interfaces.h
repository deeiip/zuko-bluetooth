#ifndef INTERFACES_H_
#define INTERFACES_H_
typedef enum iface_type{
	IFACE_BLUETOOTH
} iface_type;
typedef struct iface_desc{
	iface_type iface_typ;
} iface_desc;

iface_desc create_interface_descriptor();
#endif
