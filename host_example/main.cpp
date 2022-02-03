#include <iostream>
#include <sstream>
#include "../libqqc/methods/do_qmp2.h"

using namespace std;
using namespace libqqc;

int main (){
    cout << "Hello World!";

    Loader_qmp2 loader;
    Vault_qmp2 vault(loader);
    Do_qmp2 qmp2(vault);

    ostringstream out;
    qmp2.run(out);

    cout << out.str();

    return 0;
}
