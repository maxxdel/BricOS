#pragma once

unsigned char port_byte_in(unsigned short port);

void port_byte_out(unsigned short port, unsigned char data);

void io_wait(void);