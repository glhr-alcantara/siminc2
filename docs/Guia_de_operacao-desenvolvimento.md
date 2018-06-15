# Fluxo comum de trabalho para ser poss�vel fazer entregas parciais(HOTFIX).

Cada desenvolvedor criar� uma branch a partir da master.

## Criando uma branch a partir da branch master
    
    $ git branch
    $ git fetch
    $ git checkout master
    $ git pull origin master
    $ git checkout -b tipo-n�issue-modulo-nomeDemanda
    Eg: $ git checkout -b hotfix-007-planejamento-documentos

    master  o-------o-------------o---------------------------------------------
                     \
                      o------------------- hotfix-007-planejamento-documentos --
                    

## Fa�a commits na sua branch e envie para a branch remota no Github (origin)
    
    $ git branch
    $ git status
    $ git add docs/Guia_de_operacao-desenvolvimento.md
    $ git status
    $ git commit -m '[ FIX ] M�dulo X - funcionalidade y. Issue #007'
    $ git push origin hotfix-007-planejamento-documentos
 
    master  o-------o-------------o---------------------------------------------
                     \
                      o------o-o-o--o----- hotfix-007-planejamento-documentos --

## Atualizando a branch de teste com as altera��es mais recentes da demanda feita.

    $ git branch
    $ git fetch
    $ git checkout teste
    $ git pull origin teste
    $ git merge hotfix-007-planejamento-documentos
    $ git push origin teste

    master  o-------o-------------o---------------------------------------------
                     \
            o----o------------o---o-o---o--------- teste -----------------------
                       \               /
                        o------o-o-o--o--- hotfix-007-planejamento-documentos --


## Homologando e publicando uma vers�o para a master

Certifique-se de que a demanda est� sem diverg�ncias e se foi criada do local certo como j� foi citado acima, caso esteja tudo correto, siga os passoo a passos abaixo

(1) Caso de sucesso

    Solicite um pull request da branch em que foi feito o trabalho hotfix-007-planejamento-documentos para a branch Master e solicite a revis�o de c�digo do pacote de atualiza��o a um membro da equipe de desenvolvimento.
    
    Detalhe: Selecione apenas a branch que foi homologada e solicite um pull request para master.

    master  o-------o-------------o-----o---------------------------------------
                     \                 /
                      \               /
                       o------o-o-o--o---- hotfix-007-planejamento-documentos --

(2) Caso de falha

    Utilize sua branch para realizar as altera��es e depois prossiga o passo de mandar para teste.


## Termos e comandos usados neste documento

##### Buscar refer�ncias/meta-dados atualizados das branchs remotas(origin)
$ git fetch

##### Conferir qual � a branch local atual em que se est� trabalhando
$ git branch

##### Mudar para a branch master
$ git checkout master
    
##### Atualizar a branch master de acordo com as mudan�as no remoto (origin)
$ git pull origin master
    
##### Criar uma branch nova a partir da branch atual
$ git checkout -b tipo-n�issue-modulo-nomeDemanda

##### Conferir status da branch exibindo arquivos modificados, adicionados pra commit ou conflitos
$ git status

##### Adicionar arquivos modificados na lista pra serem enviados no pr�ximo commit
$ git add docs/Guia_de_operacao-desenvolvimento.md

##### Enviar os arquivos modificados para um marco hist�rico de munda�a na branch local
$ git commit -m '[ FIX ] M�dulo X - funcionalidade y. Issue #007'

##### Mandar seus commits e sua branch para a branch no remoto do Github (origin)
$ git push origin hotfix-007-planejamento-documentos

##### Remover branch local
$ git branch -D hotfix-007-planejamento-documentos

##### Caso tenha come�ado a modificar arquivos na branch errada e deseje esconder as altera��es pra mudar de branch ou fazer outras opera��es
$ git stash

##### Mostrar e voltar altera��es escondidas pelo $ git stash
$ git stash pop
