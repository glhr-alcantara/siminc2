<?php
/**
 * Renderizador de listagens HTML.
 *
 * @version $Id: Html.php 103789 2015-10-19 17:31:54Z VictorMachado $
 */

/**
 * Renderizador da barra de ferramentas da listagem.
 * @see Simec_Listagem_Html_Toolbar
 */
require_once(dirname(__FILE__) . '/Html/Toolbar.php');


/**
 * Renderizador base.
 * @see Simec_Listagem_Renderer_Abstract
 */
require_once(dirname(__FILE__) . '/Abstract.php');

class Simec_Listagem_Renderer_Html extends Simec_Listagem_Renderer_Abstract
{
    /**
     * @var Simec_Listagem_Renderer_Html_Toolbar Toolbar da listagem.
     */
    protected $toolbar;

    /**
     * @var array Guarda os valores do �ltimo agrupamento realiado para uma a��o durante a renderiza��o das mesmas.
     * @see Simec_Listagem_Renderer_Html::renderAcoes()
     */
    protected $acoesAgrupadasUltimoValor = array();

    /**
     * @var array Armazena uma lista de campos que s�o inclusos no formul�rio da listagem.
     */
    protected $formCampos = array();

    /**
     * @var bool Indica que deve ser exibido um bot�o importar no formul�rio.
     */
    protected $formImportar = false;

    /**
     * @var string Action que ser� utilizada pelo formul�rio.
     */
    protected $formAction = '';

    public $renderizarForm = true;

    /**
     * @var bool Indica se deve ser renderizado um prototipo de linha da listagem.
     */
    protected $renderPrototipo = false;

    /**
     * @var bool Exibe ordena��o.
     */
    protected $ordenacao = false;

    /**
     * @var bool Exibe filtros de coluna;
     */
    protected $filtros = false;

    /**
     * @var int|string Indica para o paginador qual � a p�gina atualmente carregada.
     */
    protected $paginaAtual;

    /**
     * @var string Armazena o ID do form com os filtros do relat�rio.
     */
    protected $formFiltros = '';

    /**
     * @var array Lista de campos da listagem para utiliza��o em conjunto em Simec_Listagem::SEM_RETORNO_LISTA_VAZIA.
     */
    protected $listaCampos = array();

    /**
     * @var string Armazena as configura��es de cria��o de ID de linha da listagem.
     */
    protected $configIdLinha;

    /**
     * @var array Armazena o array com as larguras das colunas da tabela.
     */
    protected $larguraColuna = array();

    /**
     * @var bool Indica se o c�digo JS da listagem j� foi inclu�do.
     */
    static protected $jsIncluido = false;

    public function setDados($dados)
    {
        if (is_null($dados)) {
            $dados = array();
        }

        return parent::setDados($dados);
    }

    /**
     * Para mais de uma coluna, passar um array no primeiro par�metro, ignorando o segundo par�metro.
     * Ex.: setLarguraColuna(array('campo' => '50%')) || setLarguraColuna('campo','50%').
     * @param string|array $campo (Coluna para sql ou chave para dados)
     * @param string $largura (Pode ser definida como desejar: X%, Ypx ...)
     */
    public function setLarguraColuna($campo, $largura = null){
        if(is_array($campo)){
            $this->larguraColuna = $campo;
            return;
        }
        $this->larguraColuna[$campo] = $largura;
    }

    /**
     * Liga e desliga a ordena��o de colunas.
     * @param bool $ordenacao
     */
    public function setOrdenacao($ordenacao)
    {
        $this->ordenacao = $ordenacao;
    }

    /**
     * Retorna se a ordena��o de colunas est� ou n�o ligada.
     * @return bool
     */
    public function getOrdenacao()
    {
        return $this->ordenacao;
    }

    /**
     * Define as configura��es de filtro.
     * @param bool|array $filtros
     */
    public function setFiltros($filtros)
    {
        if ((true === $filtros) && is_array($this->filtros)) {
            return;
        }

        $this->filtros = $filtros;
    }

    /**
     * Retorna as configura��es de filtro.
     * @return mixed
     */
    public function getFiltros()
    {
        return $this->filtros;
    }

    /**
     * Define a a��o que ser� criada para o formul�rio.
     * @param string $formAction A��o do formul�rio
     */
    public function setFormAction($formAction)
    {
        $this->formAction = $formAction;
    }

    public function setPaginaAtual($pagina)
    {
        $this->paginaAtual = $pagina;
    }

    /**
     * Armazena o nome do form que tem os filtros da listagem.
     * @param string $formid
     */
    public function setFormFiltros($formid)
    {
        $this->formFiltros = $formid;
    }

    /**
     * Ativa a cria��o de ID de linha e tamb�m configura o prefixo, se necess�rio.
     * @param string $prefixo Prefixo da cria��o de ID da TR.
     */
    public function setIdLinha($prefixo = '')
    {
        $this->configIdLinha = $prefixo;
    }

    public function setCampos(array $listaCampos)
    {
        $this->listaCampos = $listaCampos;
    }

    /**
     * Imprime o c�digo HTML da listagem.
     *
     * @uses Simec_Listagem_Renderer_Html::$jsIncluido
     */
    public function render()
    {
        // -- Inclu�ndo o arquivo de javascript de fun��es do relat�rio
        if (!self::$jsIncluido) {
            self::$jsIncluido = true;

            echo <<<HTML
<script type="text/javascript" lang="JavaScript">
jQuery(document).ready(function(){
    if (!(typeof Function == typeof window['listagem_js_carregado'])) {
        jQuery.getScript('/library/simec/js/listagem.js');
    }
});
</script>
HTML;
        }
        // -- T�tulo do relat�rio
        $this->renderTitulo();
        if ($this->renderizarForm) {
            echo <<<HTML
            <form method="post" class="form-listagem" action="{$this->formAction}" data-form-filtros="{$this->formFiltros}">
HTML;
        }
        // -- Inputs de controle da listagem
        // -- @todo Necess�rio apenas quando houver pagina��o
        echo <<<HTML
                <input type="hidden" name="listagem[p]" id="listagem-p" value="{$this->paginaAtual}" />
                <input type="hidden" name="listagem[ordenacao]" id="input_ordenacao" class="input_ordenacao" />
HTML;
        if ($this->formCampos) {
            foreach ($this->formCampos as $configCampo) {
                echo <<<HTML
<input type="{$configCampo['type']}" name="{$configCampo['name']}" id="{$configCampo['id']}" />
HTML;
            }
        }
        $qtdAcoes = $this->config->getNumeroAcoes();

        // -- Barra de ferramentas da listagem
        echo $this->renderToolbar();

        // -- Prototipo de linha
        $prototipo = $this->renderPrototipo();
        $idTabela = $this->getConfig()->getId();

        echo <<<HTML
<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem"
       id="{$idTabela}" data-qtd-acoes="{$qtdAcoes}" {$prototipo}>
HTML;
        echo $this->renderCabecalho();

        $this->renderDados();
        echo $this->renderRodape();
        echo '</table>';
        if ($this->formImportar) {
            echo <<<HTML
<button type="submit" class="btn btn-primary" id="import-data">Importar</button>
HTML;
        }
        if ($this->renderizarForm) {
            echo '</form>';
        }
    }

    /**
     * Verifica se � necessario incluir um t�tulo na listagem.
     * @see Simec_Listagem::render()
     */
    protected function renderTitulo()
    {
        if ($this->titulo) {
            echo <<<HTML
            <div class="page-header" style="margin-bottom:2px;padding-bottom:1px">
                <h4>{$this->titulo}</h4>
            </div>
HTML;
        }
    }

    protected function renderCabecalho()
    {
        // -- Se n�o houver um cabe�alho definido, pula a cria��o do cabe�aho
        if (is_null($this->getCabecalho())) {
            return '';
        }

        if ('auto' == $this->getCabecalho()) {
            $todosCampos = array_keys(current($this->dados));
            $this->config->setCabecalho(
                array_diff($todosCampos, $this->config->getColunasOcultas())
            );
        }

        // -- Verificando a quantidade de n�veis do t�tulo
        $cabecalhoComDoisNiveis = false;
        foreach ($this->getCabecalho() as $titColuna) {
            if (is_array($titColuna)) {
                $cabecalhoComDoisNiveis = true;
                break;
            }
        }

        $htmlCabecalho = '';

        // -- Faz o processamento da coluna de a��es do sistema
        $numAcoes = $this->config->getNumeroAcoes();
        if ($numAcoes) {
            $rowSpan = $cabecalhoComDoisNiveis?' rowspan="2"':'';
            $htmlCabecalho .= <<<HTML
<th colspan="{$numAcoes}"{$rowSpan}>&nbsp;</th>
HTML;
        }

        // -- Retornando o nome dos campos para associar � ordena��o das colunas
        $campos = $this->camposDoCabecalho();
        // -- Indexador de campos da query
        $numCampo = 0;

        // -- Processando o primeiro n�vel do cabecalho
        foreach ($this->getCabecalho() as $key => $titColuna) {
            if (is_array($titColuna)) {
                $qtdColunas = count($titColuna);

                $htmlCabecalho .= <<<HTML
<th colspan="{$qtdColunas}">{$key}</th>
HTML;
                $numCampo += $qtdColunas;
            } else {
                $htmlCabecalho .= $this->renderCabecalhoFolha($titColuna, $campos[$numCampo], $cabecalhoComDoisNiveis, 1);
                $numCampo++;
            }
        }

        $cabecalho = "<tr>{$htmlCabecalho}</tr>";

        // -- Faz o processamento do segundo n�vel do cabecalho
        $htmlCabecalho = '';
        // -- Indexador de campos da query
        $numCampo = 0;
        foreach ($this->getCabecalho() as $agrupColunas => $titColuna) {
            if (is_array($titColuna)) {
                foreach ($titColuna as $titulo) {
                    $htmlCabecalho .= $this->renderCabecalhoFolha($titulo, $campos[$numCampo], $cabecalhoComDoisNiveis, 2);
                    $numCampo++;
                }
                continue;
            }
            $numCampo++;
        }

        $cabecalho .= (!empty($htmlCabecalho)?"<tr>{$htmlCabecalho}</tr>":'')
                   . ($this->getFiltros()?$this->renderFiltro($campos, $numAcoes):'');

        $cabecalho = "<thead>{$cabecalho}</thead>";

        return $cabecalho;
    }

    /**
     * Renderiza uma coluna de t�tulo que representa um campo da query - pode ser filhos de agrupadores de coluna.<br />
     * No caso de uma coluna que n�o � agrupada, inclui os rowspans necess�rios para desenhar corretamente a coluna.
     *
     * @param string $titulo T�tulo da coluna.
     * @param string $nomeCampo Nome da coluna na query.
     * @param bool $cabecalhoComDoisNiveis Indica se o cabe�alho tem dois n�veis.
     * @param int $nivelAtual Indica o n�vel atual do processamento.
     * @return string
     */
    function renderCabecalhoFolha($titulo, $nomeCampo, $cabecalhoComDoisNiveis, $nivelAtual)
    {
        $rowspan = '';
        $width = '';
        if ($cabecalhoComDoisNiveis && 1 == $nivelAtual) {
            $rowspan = ' rowspan="2"';
        }
        if (array_key_exists($nomeCampo, $this->larguraColuna)){
            $width = " style=\"width: {$this->larguraColuna[$nomeCampo]};\"";
        }
        $html = <<<HTML
<th{$rowspan}{$width}>
    {$titulo}
HTML;
        if ($this->getOrdenacao()) {
            $html .= <<<HTML
    <br />
    <span class="glyphicon glyphicon-arrow-down campo_ordenacao" campo-ordenacao="{$nomeCampo}"></span>
    <span class="glyphicon glyphicon-arrow-up campo_ordenacao" campo-ordenacao="{$nomeCampo} DESC"></span>
HTML;
        }
        $html .= <<<HTML
</th>
HTML;
        return $html;
    }

    protected function renderFiltro($campos, $numAcoes)
    {
        $html = '<tr>';

        // -- Tratando as colunas de a��o
        if ($numAcoes > 0) {
            $html .= <<<HTML
<th colspan="{$numAcoes}">&nbsp;</th>
HTML;
        }

        // -- Processando cada um dos campos presentes na query
        foreach ($campos as $campo) {
            $html .= <<<HTML
<th class="campo_filtro"><input name="filtro[{$campo}]" class="form-control" value="{$this->filtros[$campo]}" /></th>
HTML;
        }

        $html .= '</tr>';
        return $html;
    }

    /**
     * Retorna uma lista com os campos da query que s�o exibidos em cada coluna.
     * @return array
     */
    protected function camposDoCabecalho()
    {
        // -- Lista completa de campos retornados pela query
        if (!empty($this->dados)) {
            // -- Lista completa de campos retornados pela query
            $campos = array_keys(current($this->dados));
        } else { // -- Utilizando qdo a listagem est� com o tratamento de vazios Simec_Listagem::SEM_REGISTRO_LISTA_VAZIA
            $campos = $this->listaCampos;
        }

        // -- Se h� alguma a��o, remove a primeira coluna, pois ela foi usada como id para as a��es
        if ($this->config->getNumeroAcoes() > 0) {
            array_shift($campos);
        }

        // -- Elimina da lista de colunas, aquelas que est�o ocultas
        $campos = array_diff($campos, $this->config->getColunasOcultas());
        return array_values($campos);
    }

    protected function renderDados($dados = null)
    {
        if (is_null($dados)) {
            $dados = $this->dados;
        }

        // -- @todo Verificar se o tipo de listagem � de FORM, para incluir os inputs
        if (empty($dados)) {
            $colspan = $this->config->getNumeroAcoes() + count($this->listaCampos);
            echo <<<HTML
    <tbody>
        <tr class="lista-vazia">
            <td colspan="{$colspan}">
            <div class="alert alert-info col-lg-8 col-lg-offset-2" role="alert">Nenhum registro encontrado</div>
            </td>
        <tr>
    <tbody>
HTML;
            return;
        }
        echo <<<BODY
    <tbody>
BODY;
        foreach ($dados as $linha => $dadosLinha) {
            $this->renderLinha($linha, $dadosLinha);
        }
        echo <<<BODY
    </tbody>
BODY;
    }

    protected function renderLinha($linha, $dadosLinha)
    {
        $classe = $this->getClasseLinha($dadosLinha, current($dadosLinha));
        $id = !is_null($this->configIdLinha)?' id="' . $this->configIdLinha . current($dadosLinha) . '"':'';

        echo <<<TR
        <tr{$classe}{$id}>
TR;
        echo $this->renderAcoes($dadosLinha, $linha);

        // -- @todo: Verificar o tipo da listagem do formulario
        if ($this->config->getNumeroAcoes()) {
            $idLinha = array_shift($dadosLinha);
        } else {
            reset($dadosLinha);
            $idLinha = current($dadosLinha);
        }
        foreach ($dadosLinha as $key => $dado) {
            if (!$this->config->colunaEstaOculta($key)) {
                // -- Verifica��o de totalizador de coluna
                $this->somarColuna($key, $dado);

                // -- Chamando fun��o de callback registrada para o campo da listagem
                $dado = $this->aplicarCallback($key, $dado, array($dadosLinha, $idLinha, array(
                    // -- Adicione novos par�metros neste array
                    'campo' => $key,
                )));
                echo <<<TD
            <td>{$dado}</td>
TD;
            }
        }

        // -- renderizando as colunas virtuais
        foreach ($this->config->getColunasVirtuais() as $coluna) {
            $retorno = $coluna($dadosLinha, $idLinha);
            echo <<<TD
            <td>{$retorno}</td>
TD;
        }

        echo <<<TR
        </tr>
TR;
    }

    /**
     * @param type $dados
     * @param type $linha
     * @return string
     */
    protected function renderAcoes($dados, $linha)
    {
        $acoesHTML = '';
        if (0 === $this->config->getNumeroAcoes()) {
            return $acoesHTML;
        }

        foreach ($this->config->getAcoes() as $acao => $configAcao) {
            if (!is_array($configAcao)) {
                $configAcao = array('func' => $configAcao);
            }

            if (!$this->config->acaoEhAgrupada($acao)) { // -- acao n�o agrupada
                $html = <<<HTML
<td class="text-center" style="width:33px">%s</td>
HTML;
                $acoesHTML .= sprintf(
                    $html,
                    $this->renderAcao($acao, $dados, $configAcao)
                );
            } elseif (!$this->acaoJahAgrupada($acao, $dados)) { // -- Agrupando o primeiro item da a��o
                // -- Guardando os valores do novo agrupamento
                $this->salvarAgrupamento($acao, $dados);
                // -- Renderiza com rowspan
                $html = <<<HTML
<td class="text-center" style="width:33px" rowspan="%d">%s</td>
HTML;
                $acoesHTML .= sprintf(
                    $html,
                    $this->calculaRowspan($acao),
                    $this->renderAcao($acao, $dados, $configAcao)
                );
            } else { // -- Tratamento de a��es j� agrupadas
                // -- n�o faz nada
            }
        }
        return $acoesHTML;
    }

    protected function renderAcao($acao, $dados, $configAcao)
    {
        $configAcao['condicao'] = $this->config->getCondicaoAcao($acao);
        $objAcao = Simec_Listagem_FactoryAcao::getAcao($acao, $configAcao);
        return (string)$objAcao->setDados($dados);
    }

    protected function acaoJahAgrupada($acao, $dados)
    {
        if (empty($this->acoesAgrupadasUltimoValor[$acao])) {
            $this->acoesAgrupadasUltimoValor[$acao] = array('dummy' => 2);
        }

        return $this->acoesAgrupadasUltimoValor[$acao] == array_intersect_assoc(
                $this->acoesAgrupadasUltimoValor[$acao],
                $dados
            );
    }

    protected function salvarAgrupamento($acao, $dados)
    {
        $this->acoesAgrupadasUltimoValor[$acao] = array_intersect_key(
            $dados,
            array_fill_keys($this->config->getAgrupamentoAcao($acao), null)
        );
    }

    protected function calculaRowspan($acao)
    {
        $ultimoAgrupamento = $this->acoesAgrupadasUltimoValor[$acao];
        $rowspan = 0;
        $achou = false;
        foreach ($this->dados as $linha) {
            $igual = ($ultimoAgrupamento == array_intersect_assoc($ultimoAgrupamento, $linha));
            // -- Se j� encontrou um elemento igual anteriormente (achou == true) e o elemento
            // -- atualmente analisado n�o � igual, finaliza a compara��o - os itens sempre devem
            // -- estar ordenado de acordo com o agrupamento - pequena otimiza��o da busca evitando
            // -- a an�lise do restante da lista uma vez que o conjunto j� foi encontrado.
            $achou = (!$achou && $igual);
            if ($achou && !$igual) {
                break;
            }
            $rowspan += (int)$igual;
        }
        return $rowspan;
    }

    /**
     * TODO: fazer o tratamento de valores considerando outros campos, quando o valor comparado for um array
     * @param $dados
     * @param $idLinha
     * @return string
     */
    protected function getClasseLinha($dados, $idLinha)
    {
        foreach ($this->config->getRegrasDeLinha() as $regra) {
            $method = strtolower($regra['op']);
            if(is_array($regra['valor'])){
                foreach($regra['valor'] as $valor){
                    if (Simec_Operacoes::$method($dados[$regra['campo']], $valor)) {
                        return <<<HTML
                            class="{$regra['classe']}"
HTML;
                    }
                }
            }

            $valor = null;
            if ($regra['valorComoCampo'] && 'id' == $regra['valor']) {
                $valor = $idLinha;
            } else if ($regra['valorComoCampo']) {
                $valor = $dados[$regra['valor']];
            } else {
                $valor = $regra['valor'];
            }

            if (Simec_Operacoes::$method($dados[$regra['campo']], $valor)) {
                return <<<HTML
                    class="{$regra['classe']}"
HTML;
            }
        }
    }

    /**
     * Cria o rodap� da listagem.
     * @todo Implementar o totalizador de coluna.
     */
    protected function renderRodape()
    {
        $qtdAcoes = $this->config->getNumeroAcoes();

        if (Simec_Listagem::TOTAL_QTD_REGISTROS == $this->config->getTotalizador()) {
            $spanDeColunas = (count($this->dados[0]) -1) + $qtdAcoes;
            $numRegistros = count($this->dados);
            echo <<<HTML
                    <tfoot>
                        <tr>
                            <td style="text-align:right" colspan="{$spanDeColunas}"><strong>Total de registros:&nbsp; $numRegistros</strong></td>
                        </tr>
                    </tfoot>
HTML;
        } elseif (Simec_Listagem::TOTAL_SOMATORIO_COLUNA == $this->config->getTotalizador()) {
            echo <<<HTML
                    <tfoot>
                        <tr>
HTML;
                            // -- Corre��o para quando tem mais de uma a��o na listagem.
                            $dadosLinha = $this->dados[0];
                            for ($i = 0; $i < $qtdAcoes; $i++) {
                                array_unshift($dadosLinha, "--a{$i}");
                            }

                            foreach ($dadosLinha as $key => $dado) {
                                if ($this->config->colunaEstaOculta($key)) {
                                    continue;
                                }
                                if (0 === $key) { // -- oO ????
                                    continue;
                                }

                                if ($this->config->colunaEhTotalizada($key)) {
                                    $valor = $this->aplicarCallback($key, $this->getTotalColuna($key));
                                    echo <<<HTML
                                            <td style="font-weight:bold;color:#428bca">{$valor}</td>
HTML;
                                } else {
                                    echo <<<HTML
                                            <td>&nbsp;</td>
HTML;
                                }
                            }
                            echo <<<HTML
                        </tr>
                    </tfoot>
HTML;
        }
    }

    protected function renderPrototipo()
    {
        if (!$this->renderPrototipo) {
            return '';
        }

        function criaParametro(&$item)
        {
            $item = "%{$item}%";
        }

        // -- Encontrando a lista de colunas da listagem
        $colunas = array();
        if (!empty($this->dados)) {
            $colunas = array_keys(current($this->dados));
        } elseif ($this->config->getListaColunas()) {
            $colunas = array_values($this->config->getListaColunas());
        } else {
            throw new Exception('N�o � poss�vel criar o prot�tipo de linha sem dados, ou sem a lista de campos.');
        }

        $arrayParametros = range(1, count($colunas));
        array_walk($arrayParametros, 'criaParametro');
        $arrayParametros = array_combine($colunas, $arrayParametros);

        // -- Gerando o HTML da lista
        ob_start();
        $this->renderLinha(0, $arrayParametros);
        return 'data-prototipo="' . htmlspecialchars(trim(ob_get_clean()), ENT_QUOTES) . '"';
    }

    /**
     * Cria, se necess�rio, uma inst�ncia da toolbar e a retorna.
     *
     * @return Simec_Listagem_Renderer_Html_Toolbar
     * @uses Simec_Listagem_Renderer_Html_Toolbar
     */
    public function getToolbar()
    {
        if (!isset($this->toolbar)) {
            $this->toolbar = new Simec_Listagem_Renderer_Html_Toolbar();
        }

        return $this->toolbar;
    }

    /**
     * Inclui o pesquisator na barra de ferramentas da listagem.
     *
     * @uses Simec_Listagem_Renderer_Html_Toolbar::PESQUISATOR
     */
    public function turnOnPesquisator()
    {
        $this->getToolbar()->add(Simec_Listagem_Renderer_Html_Toolbar::PESQUISATOR);
    }

    /**
     * Adiciona uma funcionalidade na barra de ferramentas da listagem.
     *
     * @param int,... $item Uma das funcionalidades definidas em Simec_Listagem_Renderer_Html_Toolbar.
     * @uses Simec_Listagem_Renderer_Html_Toolbar::ADICIONAR Funcionalidade de adicionar novo item.
     * @uses Simec_Listagem_Renderer_Html_Toolbar::INVERTER Funcionalidade de inverter a sele��o.
     */
    public function addToolbarItem($item)
    {
        $this->getToolbar()->add(func_get_args());
    }

    /**
     * Indica que um prototipo de linha da listagem deve ser incluso no atributo data-prototipo.
     */
    public function turnOnPrototipo()
    {
        $this->renderPrototipo = true;
    }

    /**
     * Renderiza o pesquisator - busca r�pida.
     * @return string
     */
    protected function renderToolbar()
    {
        if (isset($this->toolbar)) {
            return (string)$this->toolbar;
        }
    }

    /**
     * Adiciona um novo campo no formul�rio da listagem.
     * @param array $campos Configura��o do campo com: id, name e type.
     */
    public function addCampo(array $campos)
    {
        $this->formCampos[] = $campos;
    }

    public function mostrarImportar($mostrar = true)
    {
        $this->formImportar = $mostrar;
        return $this;
    }
}
