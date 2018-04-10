# Usando o Gerador
**Etapa 1** Selecionar um esquema.
**Etapa 2** Selecionar as tabelas.
**Etapa 3** Confirmar as informa��es adicionais: caminho, extens�o dos arquivos e prefixo da classe.

# Erro no gerador

Caso ocorrer o seguinte erro no gerador, voc� dever� criar um caminho de acordo com o diret�rio anexado no erro:

Warning - Ambiente de Desenvolvimento

fopen(/var/www/siminc2/www/gerador/arquivos_gerados/model/Acao.inc): failed to open stream: No such file or directory


Esse erro acontece pois n�o encontra um diret�rio para gerar os novos arquivos.

## Comandos
Caso n�o tenha as pastas em seu diret�rio, voc� tera que dar os seguintes comandos:
	mkdir arquivos_gerados
	cd arquivos_gerados
	mkdir form
	mkdir lista
	mkdir controller
	mkdir model
Esses comandos dever�o ser executados dentro do diret�rio simin2/www/gerador/


