#include <dns_sd.h>
int main()
{
    DNSServiceRef client_pa{};
    DNSServiceCreateConnection(&client_pa);
}
