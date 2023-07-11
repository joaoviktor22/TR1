#include "CamadaFisica.h"
#include "CamadaEnlace.h"

using namespace std;

int tipoDeControleDeErro = 2;
int tipoDeEnquadramento = 0;

void CamadaEnlaceDadosTransmissora(const vector<int>& quadro){
    vector<int> quadroEnquadrado;
    quadroEnquadrado = CamadaEnlaceDadosTransmissoraEnquadramento(quadro);
    quadroEnquadrado = CamadaEnlaceDadosTransmissoraControleDeErro(quadroEnquadrado);

    CamadaFisicaTransmissora(quadroEnquadrado);
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramento(const vector<int>& quadro){
    vector<int> quadroEnquadrado;
    switch (tipoDeEnquadramento) {
        case 0: //bit de paridade par
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(quadro);
            break;
        case 1 : //CRC
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(quadro);
            break;
        default :
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(quadro);
            break;
    }
    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErro(const vector<int>& quadro){
    vector<int> quadroEnquadrado;
    switch (tipoDeControleDeErro) {
        case 0: //bit de paridade par
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(quadro);
            break;
        case 1 : //CRC
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraControleDeErroCRC(quadro);
            break;
        case 2: //codigo de Hamming
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(quadro);
            cout << "hamming:" << endl;
            for (int bit : quadroEnquadrado) {
                cout << bit << " ";
            }
            cout << endl;
            break;
        default:
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(quadro);
            break;
    }
    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(const vector<int>& quadro){
    vector<int> quadroEnquadrado;

    int tamanhoQuadroBytes = quadro.size() / 8; // Tamanho do quadro em bytes
    cout << "tamanho: "<<tamanhoQuadroBytes << endl;

    // Converter o tamanho do quadro em bits para um vetor de bits
    vector<int> tamanhoQuadroBits;
    for (int i = 7; i >= 0; i--) {
        int bit = (tamanhoQuadroBytes >> i) & 1;
        cout << bit;
        tamanhoQuadroBits.push_back(bit);
    }

    // Adicionar o tamanho do quadro no cabeçalho
    for (int bit : tamanhoQuadroBits) {
        quadroEnquadrado.push_back(bit);
    }

    // Adicionar os bits do quadro original
    for (int bit : quadro) {
        quadroEnquadrado.push_back(bit);
    }

    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(const vector<int>& quadro){
    vector<int> quadroEnquadrado;

    // Byte de flag em bits (exemplo: 01111110)
    vector<int> flagBits = {0, 1, 1, 1, 1, 1, 1, 0};

    // Adicionar o byte de flag no início do quadro
    quadroEnquadrado.insert(quadroEnquadrado.end(), flagBits.begin(), flagBits.end());

    // Adicionar o quadro original
    quadroEnquadrado.insert(quadroEnquadrado.end(), quadro.begin(), quadro.end());

    // Adicionar o byte de flag no final do quadro
    quadroEnquadrado.insert(quadroEnquadrado.end(), flagBits.begin(), flagBits.end());

    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(const vector<int>& quadro){
    vector<int> quadroComParidade = quadro;

    // Contar o número de bits 1 no quadro
    int numBits1 = count(quadro.begin(), quadro.end(), 1);

    // Calcular o bit de paridade (0 se o número de bits 1 for par, 1 se for ímpar)
    int bitParidade = (numBits1 % 2 == 0) ? 0 : 1;

    cout << "paridade:" << bitParidade << endl;
    // Adicionar o bit de paridade ao quadro
    quadroComParidade.push_back(bitParidade);

    return quadroComParidade;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCRC(const vector<int>& quadro){
    // Polinômio gerador
    vector<int> gerador = {1, 0, 1, 1};  // Exemplo de polinômio gerador: x^3 + 1 (representado como 1011)

    // Adicionar zeros ao final do quadro para acomodar o CRC
    vector<int> quadroComZeros = quadro;
    quadroComZeros.insert(quadroComZeros.end(), gerador.size() - 1, 0);

    // Realizar a operação de divisão (XOR) entre o quadro e o polinômio gerador
    for (int i = 0; i <= quadroComZeros.size() - gerador.size(); i++) {
        if (quadroComZeros[i] == 1) {
            for (int j = 0; j < gerador.size(); j++) {
                quadroComZeros[i + j] ^= gerador[j];
            }
        }
    }

    // O resto da divisão (resultado final) é o CRC
    vector<int> crc(gerador.size() - 1);
    copy(quadroComZeros.end() - crc.size(), quadroComZeros.end(), crc.begin());

    // Adicionar o CRC ao final do quadro
    vector<int> quadroComCRC = quadro;
    quadroComCRC.insert(quadroComCRC.end(), crc.begin(), crc.end());

    return quadroComCRC;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(const vector<int>& quadro){
    int m = quadro.size();  // Número de bits de dados
    int r = 0;              // Número de bits de redundância

    // Determinar o número de bits de redundância necessários
    while ((1 << r) < m + r + 1) {
        r++;
    }

    // Calcular a posição dos bits de redundância
    vector<int> posicoesBitsRedundancia;
    for (int i = 0; i < r; i++) {
        posicoesBitsRedundancia.push_back((1 << i) - 1);
    }

    // Criar o quadro com os bits de dados e os bits de redundância
    vector<int> quadroComRedundancia;
    int posQuadro = 0;
    int posRedundancia = 0;
    for (int i = 0; i < m + r; i++) {
        if (i == posicoesBitsRedundancia[posRedundancia]) {
            quadroComRedundancia.push_back(0);  // Colocar um bit de redundância 0 temporariamente
            posRedundancia++;
        } else {
            quadroComRedundancia.push_back(quadro[posQuadro]);
            posQuadro++;
        }
    }

    // Calcular os bits de paridade
    for (int i = 0; i < r; i++) {
        int posicaoBitRedundancia = (1 << i) - 1;
        int paridade = 0;
        for (int j = 0; j < m + r; j++) {
            if (((j + 1) & (posicaoBitRedundancia + 1)) != 0) {
                paridade ^= quadroComRedundancia[j];
            }
        }
        quadroComRedundancia[posicaoBitRedundancia] = paridade;
    }

    return quadroComRedundancia;
}

void CamadaEnlaceDadosReceptora(const vector<int>& quadro){
    vector<int> quadroEnquadrado;
    quadroEnquadrado = CamadaEnlaceDadosReceptoraControleDeErro(quadro);
    quadroEnquadrado = CamadaEnlaceDadosReceptoraEnquadramento(quadroEnquadrado);

    CamadaDeAplicacaoReceptora(quadroEnquadrado);
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramento(const vector<int>& quadro){
    vector<int> quadroEnquadrado;

    switch (tipoDeEnquadramento) {
        case 0: //bit de paridade par
            quadroEnquadrado = CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(quadro);
            break;
        case 1 : //CRC
            quadroEnquadrado = CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(quadro);
            break;
        default:
            quadroEnquadrado = CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(quadro);
            break;
    }
    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosReceptoraControleDeErro(const vector<int>& quadro){
    vector<int> quadroEnquadrado;
    switch (tipoDeControleDeErro) {
        case 0: //bit de paridade par
            quadroEnquadrado = CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(quadro);
            break;
        case 1 : //CRC
            quadroEnquadrado = CamadaEnlaceDadosReceptoraControleDeErroCRC(quadro);
            break;
        case 2: //codigo de Hamming

            quadroEnquadrado = CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(quadro);
            break;
        default:
            quadroEnquadrado = CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(quadro);
            break;
    }
    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(const vector<int>& quadro) {
    vector<int> quadroOriginal = quadro; // Criar uma cópia do quadro original
    quadroOriginal.pop_back();// Remover o bit de paridade do quadro recebido
    // Contar o número de bits 1 no quadro original
    int numBits1 = count(quadroOriginal.begin(), quadroOriginal.end(), 1);
    cout << "numbits:" << numBits1 << endl;
    // Calcular o bit de paridade do quadro original (0 se o número de bits 1 for par, 1 se for ímpar)
    int bitParidadeOriginal = (numBits1 % 2 == 0) ? 0 : 1;
    cout << "paridade:" << bitParidadeOriginal << endl;
    // Verificar se houve erro comparando o bit de paridade original com o bit de paridade recebido
    bool erro = (bitParidadeOriginal != quadro.back());

    if (erro) {
        // Tratamento de erro - pode ser feito alguma ação, como retransmissão do quadro
        cout << "Erro na transmissão - Bit de paridade par não coincide!" << endl;
    } else {
        cout << "Quadro recebido corretamente - Bit de paridade par coincide!" << endl;
    }

    return quadroOriginal;
}

vector<int> CamadaEnlaceDadosReceptoraControleDeErroCRC(const vector<int>& quadro) {
    // Polinômio gerador
    vector<int> gerador = {1, 0, 1, 1};  // Exemplo de polinômio gerador: x^3 + 1 (representado como 1011)

    vector<int> quadroComCRC = quadro;
    vector<int> resto(gerador.size() - 1, 0);  // Vetor para armazenar o resto da divisão

    for (int i = 0; i < quadro.size(); i++) {
        if (quadroComCRC[i] == 1) {
            for (int j = 0; j < gerador.size(); j++) {
                quadroComCRC[i + j] ^= gerador[j];
            }
        }
    }

// Verificar se houve erro comparando o resultado da divisão (resto) com um vetor de zeros
    bool erro = any_of(resto.begin(), resto.end(), [](int bit) { return bit == 1; });

    if (erro) {
        // Tratamento de erro - pode ser feito alguma ação, como retransmissão do quadro
        cout << "Erro na transmissão - CRC não coincide!" << endl;
    } else {
        cout << "Quadro recebido corretamente - CRC coincide!" << endl;
    }

// Remover o CRC do quadro recebido
    vector<int> quadroOriginal(quadro.begin(), quadro.end() - (gerador.size() - 1));

    return quadroOriginal;
}

vector<int> CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(const vector<int>& quadro) {
    int m = quadro.size();  // Número de bits de dados
    int r = 0;              // Número de bits de redundância

    // Determinar o número de bits de redundância necessários
    while ((1 << r) < m + r + 1) {
        r++;
    }

    // Calcular a posição dos bits de redundância
    vector<int> posicoesBitsRedundancia;
    for (int i = 0; i < r; i++) {
        posicoesBitsRedundancia.push_back((1 << i) - 1);
    }
    for (int bit : quadro) {
        cout << bit << " ";
    }
    cout << endl;

    // Fazer uma cópia do quadro original para correção
    vector<int> quadroCorrigido = quadro;

    bool erro = false;
    for (int i = 0; i < r; i++) {
        int posicaoBitRedundancia = (1 << i) - 1;
        int paridade = 0;
        for (int j = posicaoBitRedundancia; j < m + r; j += (1 << (i + 1))) {
            for (int k = 0; k < (1 << i); k++) {
                if (j + k < m + r) {
                    paridade ^= quadroCorrigido[j + k];
                }
            }
        }
        if (paridade != 0) {
            erro = true;
            // Corrigir o bit de erro
            quadroCorrigido[posicaoBitRedundancia] ^= 1;
        }
    }
    for (int bit : quadroCorrigido) {
        cout << bit << " ";
    }
    cout << endl;


    if (erro) {
        // Tratamento de erro - pode ser feito alguma ação, como retransmissão do quadro
        cout << "Erro na transmissão - Código de Hamming detectou um erro e corrigiu!" << endl;
    } else {
        cout << "Quadro recebido corretamente - Código de Hamming não detectou erro!" << endl;
    }

    // Remover os bits de redundância do quadro corrigido
    vector<int> quadroOriginal;
    int posQuadro = 0;
    int posRedundancia = 0;
    for (int i = 0; i < m + r; i++) {
        if (i != posicoesBitsRedundancia[posRedundancia]) {
            quadroOriginal.push_back(quadroCorrigido[i]);
            posQuadro++;
        } else {
            posRedundancia++;
        }
    }
    for (int bit : quadroOriginal) {
        cout << bit << " ";
    }
    cout << endl;

    return quadroOriginal;
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(const vector<int>& quadro){
    vector<int> quadroOriginal;

    // Verificar se há bits suficientes para o tamanho do quadro no cabeçalho
    if (quadro.size() >= 8) {
        // Extrair o tamanho do quadro em bytes do cabeçalho
        int tamanhoQuadroBytes = 0;
        for (int i = 0; i < 8; i++) {
            tamanhoQuadroBytes = (tamanhoQuadroBytes << 1) | quadro[i];
        }

        // Verificar se há bits suficientes para o quadro completo (incluindo o cabeçalho)
        if (quadro.size() >= 8 + tamanhoQuadroBytes * 8) {
            // Extrair os bits do quadro original
            for (int i = 8; i < 8 + tamanhoQuadroBytes * 8; i++) {
                quadroOriginal.push_back(quadro[i]);
            }
        }
    }
    return quadroOriginal;
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(const vector<int>& quadro){
    vector<int> quadroOriginal;

    // Byte de flag em bits (01111110)
    vector<int> flagBits = {0, 1, 1, 1, 1, 1, 1, 0};

    // Verificar se o quadro começa e termina com o byte de flag
    if (quadro.size() >= 2 * flagBits.size() && equal(quadro.begin(), quadro.begin() + flagBits.size(), flagBits.begin()) && equal(quadro.rbegin(), quadro.rbegin() + flagBits.size(), flagBits.rbegin())) {
        // Remover o byte de flag do início e fim do quadro
        auto quadroSemFlag = vector<int>(quadro.begin() + flagBits.size(), quadro.end() - flagBits.size());

        // Adicionar o quadro sem flags ao quadro original
        quadroOriginal.insert(quadroOriginal.end(), quadroSemFlag.begin(), quadroSemFlag.end());
    }
    return quadroOriginal;
}