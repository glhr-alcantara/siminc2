# Workflow do Kanban

## Zenhub

<https://www.zenhub.com/>

Como ferramenta, utiliza-se o Zenhub, que facilita a comunica��o
com o github e torna o processo de desenvolvimento mais f�cil.


## Entendendo o processo

No projeto SIMINC2, as demandas s�o solicitadas e entregues por Sprints, da
metodologia Scrum, geralmente, quinzenalmente.
<br>Cada Sprint cont�m um pacote de demandas que o demandante faz
para o Time de Desenvolvimento.


## Entendendo as prateleiras

### Pr�xima Sprint

Cont�m apenas as demandas que o demandante solicitar� para a Sprint seguinte.
Apenas o demandante move as Issue's(card's) para as outras prateleiras.


### Prateleira

Cont�m todas as demandas que o demandante solicitar� ao Time de Desenvolvimento.


### A Fazer

Cont�m todas as demandas que o Time de Desenvolvimento atender�.<br>
<b>Obs:</b> Quando algum membro do Time de Desenvolvimento for realizar a demanda,
o mesmo dever� mover o card(Issue) para a prateleira de <b>Fazendo</b>


### Fazendo

Cont�m todas as demandas que o Time de Desenvolvimento est� atendendo atualmente.<br>
<b>Obs:</b> Quando a demanda for finalizada, o respons�vel dever� mover o card(Issue) para
a prateleira de <b>Teste</b> e solicitar que outro membro do Time de Desenvolvimento possa testar para liberar
a demanda ao cliente.


### Impedimentos

Cont�m todas as demandas que n�o foram realizadas com �xito, ou seja, cont�m ressalvas.<br>
<b>Obs:</b> Quando o cliente ou algum membro do Time de Desenvolvimento mover algum card(Issue) para este item da
prateleira, o mesmo dever� especificar no card(Issue) o motivo do Impedimento, para que o
respons�vel pelo desenvolvimento possa corrigir a demanda. Ap�s a corre��o, p�r a demanda para
<b>Teste</b> e seguir com o Fluxo a partir do teste.


### Teste

Cont�m todas as demandas que necessitam de teste para serem liberadas ao cliente.<br>
<b>Obs:</b> O respons�vel pela verifica��o do teste dever� ser, preferencialmente, algum membro
que n�o participou do desenvolvimento da demanda.<br>
Ap�s a corre��o, o respons�vel pelo teste dever� mover a demanda para a prateleira <b>Feito</b>
e exibir a mensagem 'Testado em Homologa��o : ', informando tamb�m a url de onde ele realizou o teste <br>

<b>Eg:</b> Testado em Homologa��o :http://homologasiminc2.cultura.gov.br/planacomorc/planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A


### Feito

Cont�m todas as demandas que j� foram feitas.<br>
<b>Obs:</b> Neste Item de prateleira, o cliente far� o seu teste, onde caso a demanda n�o esteja de acordo
o mesmo mudar� o card(Issue) para <b>Impedimento</b> e especificar� o motivo do impedimento. Caso esteja tudo
de acordo, ele fechar� o card(Issue).


### Closed

Cont�m todas as demandas que j� foram testadas pelo cliente e est�o prontas para serem publicadas.<br>
<b>Obs:</b> Verificar se o card(Issue) possui a label de <b>PUBLICADA</b>, caso tenha, significa que
a demanda j� passou pela fase de publica��o. Caso n�o contenha, dever� ser solicitado o pull request e marcar algum
membro do Time de Desenvolvimento para revisar e verificar o c�digo antes da publica��o.<br><br>
<b>Ap�s a publica��o, adicionar a label de PUBLICADA na demanda, atrav�s da ferramenta Zenhub ou pelo pr�prio GitHub.<b/>