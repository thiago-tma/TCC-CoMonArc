/*
 * Lista de Testes do UserInterface
 * Objetivo: Gerenciar controle de Leds, Campainhas e Botões
 * 
 * Módulo inicializa GPIOs usados durante inicialização
 *      LED, Campainha e Botão
 * Módulo retorna pinos ao estado padrão ao ser destruido
 * Blink do LED com 1 repetição e 1 segundo
 *      InterfaceRun altera pino do LED
 *      Estado do pino se mantém com novo InterfaceRun se 1 segundo não passou
 *      Pino é desligado após 1 segundo
 * Chamar função de blink para pino com operação em andamento inicia nova operação imediatamente
 *      Blink do LED com 1 repetição e 1 segundo ON, passa 500ms e é chamada novamente
 * Blink do buzzer com 3 repetições, 500ms ligado e 100ms desligado
 *      Operação é feita no pino correto
 *      Verificação do pino em intervalos de 50 ms
 * Debounce do estado do botão (botão deve manter estado fixo por 50 ms para ser válido)
 * Transição do botão de baixo para alto (pino de baixo para alto) ativa função atrelada
 * 
*/