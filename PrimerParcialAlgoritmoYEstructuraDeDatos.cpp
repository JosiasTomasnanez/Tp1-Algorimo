#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

/**
 * @class Nodo
 * @brief Representa un nodo en una lista enlazada.
 * @tparam T Tipo de dato almacenado en el nodo.
 */
template <class T> class Nodo {
private:
  T dato;     ///< Dato almacenado en el nodo.
  Nodo *next; ///< Puntero al siguiente nodo.
public:
  Nodo() { next = NULL; };
  Nodo(T a) {
    dato = a;
    next = NULL;
  };
  void set_dato(T a) { dato = a; };
  void set_next(Nodo *n) { next = n; };
  T get_dato() { return dato; };
  Nodo *get_next() { return next; };
  bool es_vacio() { return next == NULL; }
};

/**
 * @class Lista
 * @brief Implementa una lista enlazada genérica.
 * @tparam T Tipo de dato almacenado en los nodos de la lista.
 */
template <class T> class Lista {
private:
  Nodo<T> *czo; ///< Puntero al primer nodo de la lista.

  void addO(T d, Nodo<T> *ant);
  void borrarD(T d, Nodo<T> *ant);

public:
  Lista() { czo = new Nodo<T>(); };
  Lista(Nodo<T> *n) { czo = n; };
  //~Lista(void);
  void add(T d); // sumar nodos a la lista
  bool esvacia(void);
  T cabeza(void);     // retorna el dato del primer nodo
  Lista *resto(void); // retorna el puntero al "resto" de la lista
                      // resto= lo que queda de la lista sin la cabeza
  string toPrint(string p);
  void impre(void);
  T suma(T i);
  int size();
  void borrarTodo(void);
  bool esta(T d);                            // detecta si d esta en la lista
  void borrarDato(T d) { borrarD(d, NULL); } // borra el nodo que contiene d
  void borrar(void);                         // borra la cabeza
  void borrar_last();                        // borra el ultimo
  void concat(Lista<T> *l1); // le transfiere los datos de l1 a this
  Lista<T> *copy(void);      // hace una copia de la lista
  void tomar(int n); // deja "vivos" los n primeros nodos y borra el resto
  void addOrdenado(T d) {
    addO(d, NULL);
  }; // sumar nodos a la lista Ordenados de menor a MAYOR
  T last(void);
};

/**
 * @brief Añade un nuevo nodo al inicio de la lista.
 * @param d El dato que se almacenará en el nuevo nodo.
 */
template <class T>
void Lista<T>::add(T d) // 100
{
  Nodo<T> *nuevo = new Nodo<T>(d);
  nuevo->set_next(czo);
  czo = nuevo;
}

/**
 * @brief Verifica si la lista está vacía.
 * @return true si la lista está vacía, false en caso contrario.
 */
template <class T> bool Lista<T>::esvacia(void) { return czo->es_vacio(); }

/**
 * @brief Obtiene el dato almacenado en el nodo cabeza de la lista.
 * @return El dato del nodo cabeza o NULL si la lista está vacía.
 */
template <class T> T Lista<T>::cabeza(void) {
  if (this->esvacia()) {
    return T();
  }
  return czo->get_dato();
}

/**
 * @brief Devuelve una nueva lista que representa el resto de la lista
 * (excluyendo la cabeza).
 * @return Un puntero a una nueva lista que contiene los nodos después de la
 * cabeza.
 */
template <class T> Lista<T> *Lista<T>::resto(void) {
  Lista *l = new Lista(czo->get_next());
  return (l);
}

/**
 * @brief Convierte la lista en una representación de cadena concatenando los
 * datos de los nodos.
 * @param p Una cadena inicial que se concatenará con los datos de los nodos.
 * @return Una cadena que representa los elementos de la lista.
 */
template <class T> string Lista<T>::toPrint(string p) {
  if (this->esvacia()) {
    return p;
  } else {
    // Llamada recursiva primero para el resto de la lista
    p = this->resto()->toPrint(p);
    // Luego añadir la cabeza al resultado
    ostringstream stm;
    stm << p << this->cabeza();
    return stm.str();
  }
}

/**
 * @brief Calcula la suma de los datos de todos los nodos de la lista.
 * @param i Un valor inicial para la suma.
 * @return La suma de los datos de los nodos más el valor inicial.
 */
template <class T> T Lista<T>::suma(T i) { // cout<<" i al entrar= "<<i<<endl;
  if (this->esvacia()) {
    return i;
  } else {

    // cout<<"this->cabeza()= "<<this->cabeza()<<endl;
    return this->resto()->suma(i + this->cabeza());
  }
}

/**
 * @brief Calcula el tamaño de la lista (cantidad de nodos).
 * @return El número de nodos en la lista.
 */
template <class T> int Lista<T>::size() {
  if (this->esvacia())
    return 0;
  return 1 + this->resto()->size();
}

/**
 * @brief Borra el nodo cabeza de la lista.
 */
template <class T> void Lista<T>::borrar(void) { // borra el nodo cabeza
  if (!this->esvacia()) {
    Nodo<T> *tmp = czo;
    czo = czo->get_next();
    delete tmp;
  }
}

/**
 * @brief Elimina todos los nodos de la lista.
 */
template <class T> void Lista<T>::borrarTodo(void) {
  while (!this->esvacia()) {
    borrar();
  }
}

/**
 * @brief Borra el último nodo de la lista.
 */
template <class T> void Lista<T>::borrar_last() { // borra el ultimo nodo
  if (!this->esvacia()) {
    if ((czo->get_next())->get_next() == NULL) {
      delete czo->get_next();
      czo->set_next(NULL);
    } else
      this->resto()->borrar_last();
  }
}

/**
 * @brief Concatena otra lista al final de la actual.
 * @param l1 Puntero a la lista que se concatenará.
 */
template <class T> void Lista<T>::concat(Lista<T> *l1) {
  if (!(l1->esvacia())) {
    // Primero, concatena el resto de la lista l1
    this->concat(l1->resto());
    // Luego, añade la cabeza de l1
    this->add(l1->cabeza());
  }
}

/**
 * @brief Crea una copia de la lista actual.
 * @return Un puntero a una nueva lista que es una copia de la actual.
 */
template <class T> Lista<T> *Lista<T>::copy(void) {
  Lista<T> *aux = new Lista();
  aux->concat(this);
  return aux;
}

/**
 * @brief Mantiene los primeros n nodos de la lista y elimina el resto.
 * @param n El número de nodos que deben permanecer en la lista.
 */
template <class T>
void Lista<T>::tomar(
    int n) { // deja "vivos" los n primeros nodos y borra el resto
  if (this->size() > n) {
    this->borrar_last();
    this->tomar(n);
  }
}

/**
 * @brief Imprime los datos de todos los nodos de la lista en la consola.
 */
template <class T> void Lista<T>::impre(void) {
  Nodo<T> *aux;
  aux = czo;
  while (aux->get_next() != NULL) {
    cout << aux->get_dato();
    aux = aux->get_next();
  }
}

/**
 * @brief Inserta un dato en la lista manteniendo el orden.
 * @param d El dato a insertar.
 * @param ant Puntero al nodo anterior al nodo actual.
 */
template <class T> void Lista<T>::addO(T d, Nodo<T> *ant) {
  if (this->esvacia()) { // el primero
    this->add(d);
  } else {
    if (d < czo->get_dato()) {
      if (ant == NULL) { // al principio
        this->add(d);
      } else { // entre medio
        Nodo<T> *nuevo = new Nodo<T>(d);
        nuevo->set_next(ant->get_next());
        ant->set_next(nuevo);
      }
    } else {
      if ((czo->get_next())->get_next() == NULL) { // al final
        Nodo<T> *nuevo = new Nodo<T>(d);
        nuevo->set_next(czo->get_next());
        czo->set_next(nuevo);

      } else
        this->resto()->addO(d, czo);
    }
  }
}

/**
 * @brief Verifica si un dato está presente en la lista.
 * @param d El dato a buscar.
 * @return true si el dato está en la lista, false en caso contrario.
 */
template <class T> bool Lista<T>::esta(T d) { // busca d en la lista
  if (this->esvacia())
    return false;
  if (this->cabeza() == d)
    return true;

  return this->resto()->esta(d);
}

/**
 * @brief Elimina un nodo que contiene un dato específico de la lista.
 * @param d El dato que se desea eliminar.
 * @param ant Puntero al nodo anterior al nodo actual.
 */
template <class T> void Lista<T>::borrarD(T d, Nodo<T> *ant) {
  if (!this->esvacia()) {
    if (d == this->cabeza()) {
      if (ant == NULL) { // al principio
        this->borrar();
      } else { // entre medio
        ant->set_next(czo->get_next());
        delete czo;
      }
    } else
      this->resto()->borrarD(d, czo);
  }
}

/**
 * @class Pila
 * @brief Clase que representa una pila basada en la clase Lista.
 */
template <class T> class Pila : public Lista<T> {
public:
  Pila() : Lista<T>(){};
  void apilar(T d) { this->add(d); }
  T tope(void) { return this->cabeza(); }
  void desapilar(void) { this->borrar(); }
  bool pilavacia() { return this->esvacia(); }
};

/**
 * @class Cola
 * @brief Clase que representa una cola basada en la clase Lista.
 */
template <class T> class Cola : public Lista<T> {
public:
  Cola() : Lista<T>(){}; // Llamamos al constructor de la clase base
  T tope() { return this->cabeza(); } // analizar si es cabeza o last
  bool colaVacia(void) { return this->esvacia(); }
  void unirCola(Cola<T> *c) { this->concat(c); }
  void encolar(T a) { this->add(a); }
  void desencolar(void) { this->borrar_last(); }
  T ultimo(void) { return this->last(); }
  string imprimir(string s) { return this->toPrint(s); }
};

/***********************************************************************
************************************************************************ */

/**
 * @enum Modo
 * @brief Representa los modos posibles dentro de una expresión llave-valor.
 */
enum Modo {
  LLAVE, ///< Indica que se está dentro de una estructura.
  VALOR    ///< Indica que se está procesando un valor.
};

/**
 * @class ExpresionLlaveValor
 * @brief Clase para manejar y validar expresiones en formato llave-valor.
 */
class ExpresionLlaveValor {
private:
  Cola<char>
      *bufferLV; ///< Almacena los caracteres de la expresión llave-valor.
  Pila<char>
      *pila; ///< Pila utilizada para validar la estructura de la expresión.

public:
  ExpresionLlaveValor() {
    bufferLV = new Cola<char>();
    pila = new Pila<char>();
  }
  ~ExpresionLlaveValor() {
    delete bufferLV;
    delete pila;
  }
  Cola<char> *getBuffer(void) { return bufferLV; }
  bool validarExpresion(ifstream &archivo);
  void toPrint(ofstream &archivo);
};

/**
 * @brief Valida una expresión en formato llave-valor leyendo desde un archivo.
 *
 * Este método verifica que la expresión comience con una comilla de apertura
 * (`"`) y se asegure de que las comillas de apertura y cierre estén
 * balanceadas. Los caracteres que se encuentran dentro de las comillas se
 * almacenan en el buffer.
 *
 * @param archivo Archivo de entrada desde el cual se lee la expresión.
 * @return true si la expresión es válida, false en caso contrario.
 *
 * @note Genera un mensaje de error si no se cierran las comillas adecuadamente.
 */
bool ExpresionLlaveValor::validarExpresion(ifstream &archivo) {
  char c;
  pila->apilar('"');
  while (archivo >> c) {
    if (c == '"') {

      pila->desapilar();
      break;
    } else {
      bufferLV->encolar(c);
    }
  }
  if (!pila->pilavacia()) {
    cerr << "Error: No se cerraron comillas";
    return false;
  }
  return true;
}

/**
 * @class ExpresionLlaveJSON
 * @brief Clase para manejar y validar expresiones en formato llave-JSON.
 */
class ExpresionLlaveJSON {
private:
  Cola<char> *buffer; ///< Almacena los caracteres de la expresión llave-JSON.

public:
  ExpresionLlaveJSON() { buffer = new Cola<char>(); }
  ~ExpresionLlaveJSON() { delete buffer; }
  Cola<char> *getBuffer(void) { return buffer; }
  bool validarExpresion(ifstream &archivo);
  void toPrint(ofstream &archivo);
};

/**
 * @class ExpresionLlaveLista
 * @brief Clase para manejar y validar expresiones en formato llave-lista.
 */
class ExpresionLlaveLista {
private:
  Cola<char> *buffer; ///< Almacena los caracteres de la expresión llave-lista.
  Pila<char> *pila; ///< Pila utilizada para validar la estructura de la lista.
  Cola<string> *valores; ///< Cola que almacena los valores de la lista.

public:
  ExpresionLlaveLista() {
    buffer = new Cola<char>();
    pila = new Pila<char>();
    valores = new Cola<string>();
  }
  ~ExpresionLlaveLista() {
    delete buffer;
    delete pila;
    delete valores;
  }
  bool validarExpresion(ifstream &archivo);
  Cola<char> *getBuffer() { return buffer; }
  void toPrint(ofstream &archivo);
};

/**
 * @brief Valida una expresión en formato llave-lista leyendo desde un archivo.
 *
 * Este método valida que las listas comiencen con un corchete de apertura (`[`)
 * y terminen con un corchete de cierre (`]`). Asegura el balance adecuado de
 * comillas dobles (`"`) y corchetes, verificando además que los valores en la
 * lista estén separados por comas (`.`). Los caracteres válidos dentro de los
 * corchetes o comillas se almacenan en el buffer y los valores completos en la
 * cola de valores.
 *
 * @param archivo Archivo de entrada desde el cual se lee la expresión.
 * @return true si la expresión es válida, false en caso contrario.
 *
 * @note Genera mensajes de error en los siguientes casos:
 * - Apertura de corchetes dentro de otro corchete.
 * - Caracteres fuera de las comillas o corchetes.
 * - Cierre de corchetes que no tienen apertura correspondiente.
 * - Cierre de comillas sin apertura correspondiente.
 * - Coma en ubicación no válida o al final de la lista.
 * - Strings consecutivos sin separación por coma.
 */
bool ExpresionLlaveLista::validarExpresion(ifstream &archivo) {
  char c;
  bool comahabilitada = false;
  Cola<char> *aux = new Cola<char>();
  pila->apilar('[');

  while (archivo >> c) {

    if (c == ' ' && pila->tope() != '"') {
      continue;
    }

    if (c == '[' && pila->tope() != '"') {
      cerr << "Error: No se puede abrir corchete adentro de otro corchete";
      return false;
    }

    else if (c != '"' && c != ',' && c != ']' && pila->tope() != '"') {
      cerr << "Error: hay caracteres fuera de las comillas o corchetes";
      return false;
    }

    else if (c == ']' && pila->tope() != '"') {
      if (!comahabilitada) {
        cerr << "Error: No puede terminar la lista con una coma";
        return false;
      }
      if (pila->tope() == '[') {
        pila->desapilar();
        break;
      } else {
        cerr << "Error: No se puede cerrar corchete que no se abrio";
        return false;
      }
    } else if (c == '"') {
      if (pila->tope() == '"') { // Cerrar comillas.
        pila->desapilar();
        comahabilitada = true; // Se habilita para esperar una coma.
      } else if (pila->tope() == '[') {
        if (comahabilitada) {
          cerr << "Error: No puede haber dos strings seguidos sin una coma "
                  "entre ellos."
               << endl;
          return false;
        }
        // Abrir comillas.
        pila->apilar(c);
      } else {
        cerr << "Error: No se puede abrir/cerrar comillas en este contexto."
             << endl;
        return false;
      }
    }

    else if (c == ',') {
      if (comahabilitada) {
        buffer->encolar(c);
        valores->encolar(aux->toPrint(""));
        aux->borrarTodo();
        comahabilitada = false;
      } else {
        cerr << "Error: Error en la ubicacion de la coma";
        return false;
      }
    } else {
      aux->encolar(c);
      buffer->encolar(c);
    }
  }

  if (!pila->pilavacia()) {
    cerr << "Error: corchete o comillas no cerradas";
    return false;
  }

  valores->encolar(aux->toPrint(""));
  aux->borrarTodo();
  comahabilitada = false;
  return true;
}

/**
 * @class ExpresionJsonCompleta
 * @brief Clase para validar y analizar expresiones JSON completas.
 *
 * Esta clase proporciona métodos para analizar y validar una expresión JSON
 * leída desde un archivo. Permite verificar la sintaxis correcta de llaves,
 * comillas, y otros elementos JSON, además de almacenar el resultado en un
 * buffer.
 */
class ExpresionJsonCompleta {
private:
  Cola<char> *buffer; ///< Buffer para almacenar la representación de la
                      ///< expresión JSON validada.
  Pila<char> *pila;   ///< Pila para manejar la validación de llaves y comillas.
  bool llavehabilitada; ///< Bandera para controlar si se permite abrir nuevas
                        ///< llaves.

public:
  ExpresionJsonCompleta() {
    buffer = new Cola<char>;
    pila = new Pila<char>();
    llavehabilitada = true;
  }
  ~ExpresionJsonCompleta() {
    delete buffer;
    delete pila;
  }
  Cola<char> *getBuffer() { return buffer; }
  Pila<char> *getPila() { return pila; }
  void setLlaveHabilitada(bool b) { llavehabilitada = b; }
  bool validarExpresion(ifstream &archivo);
  bool validar(ifstream &archivo);
  void toPrint() { buffer->toPrint(""); }
};

/**
 * @brief Valida una expresión JSON en un archivo de entrada y verifica que no
 * haya caracteres fuera de las llaves principales al final de la expresión.
 *
 * @param archivo Referencia al flujo de entrada `ifstream` que contiene la
 * expresión JSON a validar.
 * @return true Si la expresión JSON es válida y no hay caracteres adicionales
 * fuera de las llaves.
 * @return false Si la expresión JSON no es válida o si se encuentran caracteres
 * fuera de las llaves principales.
 *
 * @details Este método utiliza el método `validarExpresion` para validar la
 * estructura de la expresión JSON dentro de las llaves principales. Una vez
 * validada la estructura, se asegura de que no queden caracteres significativos
 * (diferentes de espacios) fuera de las llaves principales en el flujo de
 * entrada. Si se encuentran caracteres adicionales, el método emite un mensaje
 * de error por la salida de error estándar.
 */
bool ExpresionJsonCompleta::validar(ifstream &archivo) {
  bool aux = validarExpresion(archivo);
  if(aux==false){
    return false;
  }
  char c;
  if (archivo.eof()) {
    return aux;
  }
  while (archivo >> c) {
    if (c == ' ')
      continue;
    cerr << "Error: Hay caracteres fuera de las llaves principales";
    return false;
  }
  return aux;
}

/**
 * @brief Valida una expresión JSON completa desde un archivo.
 *
 * Este método procesa y verifica cada elemento de la expresión JSON,
 * controlando errores comunes como caracteres fuera de contexto, llaves o
 * comillas no cerradas, y errores en la ubicación de comas.
 *
 * @param archivo Referencia a un flujo de entrada que contiene la expresión
 * JSON.
 * @return true si la expresión es válida, false en caso contrario.
 */
bool ExpresionJsonCompleta::validarExpresion(ifstream &archivo) {
  // Verifica si el archivo esta abierto correctamente.
  if (!archivo.is_open()) {
    cerr << "Error: El archivo no se pudo abrir." << endl;
    return false;
  }

  // Variables auxiliares para controlar el estado de la validacion.
  bool dospuntoshabilitado = false; // Para controlar si se ha encontrado dos
                                    // puntos después de una clave.
  Modo modo = LLAVE;              // Modo de analisis actual: LLAVE o VALOR.
  char c; // Variable para almacenar el caracter leido.
  bool llaveterminada = false;
  // Iterar sobre cada caracter del archivo.
  while (archivo >> c) {
    // Ignorar espacios fuera de las comillas.
    if (c == ' ' && pila->tope() != '"') {
      continue;
    }

    // Si el modo es VALOR, identificar que clase continuara el analisis.
    if (modo == VALOR) {
      // Caso de valor tipo JSON (subobjeto).
      if (c == '{') {
        buffer->encolar('(');
        ExpresionLlaveJSON exp;
        if (!exp.validarExpresion(archivo)) {
          return false;
        }
        buffer->unirCola(exp.getBuffer());
        buffer->encolar(')');
      }
      // Caso de valor tipo string.
      else if (c == '"') {
        ExpresionLlaveValor exp;
        if (!exp.validarExpresion(archivo)) {
          return false;
        }
        buffer->unirCola(exp.getBuffer());

      }
      // Caso de valor tipo lista.
      else if (c == '[') {
        ExpresionLlaveLista exp;
        if (!exp.validarExpresion(archivo)) {
          return false;
        }
        buffer->encolar(c);
        buffer->unirCola(exp.getBuffer());
        buffer->encolar(']');
      }
      // Si no es un tipo valido de valor, se lanza error.
      else {
        cerr << "Error: Formato del valor incorrecto." << endl;
        return false;
      }
      // Volver al modo LLAVE despues de validar el valor.
      modo = LLAVE;
      llaveterminada = true;
      continue;
    }

    // Validar que no haya caracteres fuera de las llaves principales.
    if (c != '{' && pila->pilavacia()) {
      cerr << "Error: Hay caracteres fuera de las llaves principales." << endl;
      return false;
    }

    // Manejar la apertura de una nueva llave '{'.
    else if (c == '{' && pila->tope() != '"') {
      if (llavehabilitada) {
        pila->apilar(c);
        llavehabilitada = false;
      } else {
        cerr << "Llave de apertura en lugar incorrecto";
        return false;
      }
    }

    // Manejar el cierre de una llave '}'.
    else if (c == '}' && pila->tope() != '"') {
      if (pila->tope() != '{') {
        cerr << "Error: Se intenta cerrar una llave que no se abrio." << endl;
        return false;
      }
      pila->desapilar();
      if (pila->pilavacia()) {
        break;
      }
    } else if (llaveterminada && c != ',') {
      cerr << "Error: se nesesita comas entre subexpresiones";
      return false;
    } else if (c == ',' && pila->tope() != '"') {
      if (llaveterminada) {
        buffer->encolar('\n');
        llaveterminada = false;
        continue;
      }
      cerr << "Error:lugar de la coma equivocado";
      return false;
    }

    // Manejo de comillas '"' en modo LLAVE o COMILLAS.
    else if (c == '"') {
      if (!dospuntoshabilitado) {
        if (pila->tope() == '"') {
          pila->desapilar();
          dospuntoshabilitado = true; // Se espera que el siguiente sea un ':'.
        } else {
          pila->apilar(c);
        }
      } else { // Error si se abren comillas despues de habilitar dos puntos.
        cerr << "Error: Se abrio nuevas comillas en lugar de dos puntos."
             << endl;
        return false;
      }
    }

    // Manejar los dos puntos ':' para habilitar el valor.
    else if (c == ':' && pila->tope() != '"' && dospuntoshabilitado) {
      dospuntoshabilitado = false;
      llavehabilitada = true;
      modo = VALOR;
      buffer->encolar(c);
    } else if (pila->tope() != '"') {
      cerr << "Error: hay caracteres fuera de las comillas." << endl;
      return false;
    } else {
      buffer->encolar(c);
    }
  }

  // Verificar si todas las llaves fueron cerradas.
  if (!pila->pilavacia()) {
    cerr << "Error: Hay llave/s sin cerrar." << endl;
    return false;
  }
  return true;
}

/**
 * @brief Valida una subexpresión JSON dentro de una llave.
 *
 * Este método se utiliza para analizar una subexpresión dentro de una llave
 * JSON. Puede incluir objetos JSON anidados, listas o valores simples.
 *
 * @param archivo Referencia al flujo de entrada que contiene la expresión.
 * @return true si la subexpresión es válida, false en caso contrario.
 */
bool ExpresionLlaveJSON::validarExpresion(ifstream &archivo) {
  ExpresionJsonCompleta *exp = new ExpresionJsonCompleta();
  exp->getPila()->apilar('{');
  exp->setLlaveHabilitada(false);
  if (exp->validarExpresion(archivo)) {
    buffer->unirCola(exp->getBuffer());
    delete exp;
    return true;
  }
  delete exp;
  return false;
}

/**
 * @file PrimerParcialAlgoritmoYEstructuraDeDatos.cpp
 * @brief Archivo principal para probar la validación de expresiones JSON.
 */

/**
 * @brief Función principal del programa.
 *
 * Este programa lee una expresión JSON desde un archivo, valida su sintaxis
 * utilizando la clase `ExpresionJsonCompleta` y guarda el resultado en otro
 * archivo.
 *
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Arreglo de punteros a los argumentos de línea de comandos. El
 * primer argumento puede ser el nombre del archivo JSON a procesar.
 * @return Código de salida del programa:
 *         - 0 si la validación y el guardado se realizan con éxito.
 *         - -1 si ocurre un error durante el proceso.
 */
int main(int argc, char *argv[]) {
  string carpeta = "jsontexts/";
  string direccion = "json.json";
  ofstream archivoescritura("jsonValidacion.txt");
   if (!archivoescritura) {
    cerr << "no se pudo abrir el archivo al cual queremos escribir";
    exit(EXIT_FAILURE);
  }
  streambuf* cerr_buffer = cerr.rdbuf();
  cerr.rdbuf(archivoescritura.rdbuf());


  if (argc > 1) {
    direccion = argv[1];
  }
  ifstream archivo(carpeta + direccion);
  if (!archivo) {
    cerr << "no se pudo abrir el archivo" << endl;
   exit(EXIT_FAILURE);
  }
  ExpresionJsonCompleta *exp = new ExpresionJsonCompleta();
  if (!exp->validar(archivo)) {
    archivoescritura<< endl << "El archivo Json no es valido." <<endl;
    exit(EXIT_FAILURE);
  }
  archivoescritura << "El archivo Json es valido."<<endl;
  archivoescritura << exp->getBuffer()->imprimir("");
  delete exp;
  exit(EXIT_SUCCESS);
}
