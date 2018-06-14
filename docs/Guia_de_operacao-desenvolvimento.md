# Fluxo comum de trabalho

Cada desenvolvedor criar� uma branch a partir da master.

## Criando uma branch a partir da branch master
    
    $ git checkout master
    $ git pull origin master
    $ git checkout -b tipo-modulo-n�issue-nomeCurto
    Eg: $ git checkout -b hotfix-007-planejamento-documentos


    master   o-------------------------------
                  |
                        o------- teste --------------
                   \
                    o--- hotfix-007-planejamento-documentos
                    

## Fa�a commits na sua branch e envie para o github (origin)
    $ git add .
    $ git commit -m '[ FIX ] M�dulo X - funcionalidade x. Issue #007'
    $ git push origin hotfix-007-planejamento-documentos

    master   o-----------------------------------------
             |   \
             |    o---------------- teste ---------------
             \
              o---o----o----- hotfix-007-planejamento-documentos ---

## Atualizando a branch de teste com as altera��es mais recentes da demanda feita.

    $ git pull origin teste
    $ git merge origin hotfix-007-planejamento-documentos
    $ git push origin teste

    master   o----------------------------------------------
              \
               o-----o----o----o--- teste -----------------
                \               \
                 o---o----o------o--- origin hotfix-007-planejamento-documentos ---


## Homologando e publicando uma vers�o para a master

Certifique-se de que a demanda est� sem diverg�ncias e se foi criada do local certo como j� foi citado acima, caso esteja tudo correto, siga os passoo a passos abaixo

(1) Caso de sucesso

    Solicite um pull request [aqui](https://github.com/culturagovbr/siminc2/branches/all) e adicione a revis�o de um membro 
    
    Detalhe: Selecione apenas a branch que foi homologada e solicite um pull request para master.

(2) Caso de falha

    Utilize sua branch para realizar as altera��es e depois prossiga o passo de mandar para teste.


## Termos e comandos usados neste documento

##### Muda para a branch master
$ git checkout master
    
##### Atualiza a branch master de acordo com as mudan�as no remoto (origin)
$ git pull origin master
    
##### Cria uma branch com o padr�o usado no SIMINC2
$ git checkout -b tipo-modulo-n�issue-nomeDemanda

##### Adiciona as altera��es feitas
$ git add docs/Guia_de_operacao-desenvolvimento.md

##### Remover branch local
$ git branch -D nomebranch

##### Faz download dos �ltimos commits de todas as branch's remoto (origin)
$ git fetch

##### Mandar sua branch para remoto (origin)
$ git push -u --set-upstream origin nomeDemanda