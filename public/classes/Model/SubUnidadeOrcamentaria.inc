<?php
/**
 * Classe de mapeamento da entidade public.subunidadeorcamentaria
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-08-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Subunidadeorcamentaria
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_SubUnidadeOrcamentaria extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.subunidadeorcamentaria';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'suoid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'unoid' => array('tabela' => 'unidadeorcamentaria', 'pk' => 'unoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'suoid' => null,
        'unoid' => null,
        'suocod' => null,
        'suosigla' => null,
        'suonome' => null,
        'suostatus' => null,
        'suoflaglimiteliberado' => null,
        'suocodigopi' => null,
    );

    /**
     * Fun��o recuperarTodos
     * M�todo usado para recuperar todos os registros do banco, seguinto par�metros
     * @param string $stCampos - String contendo nomes dos campos a serem carregados
     * @param array $arClausulas - Array contendo dados da cl�usula where
     * @param string $stOrdem - String contendo dados da cl�usula ordey by
     * @param array $opcoes Op��es adicionais do m�todo.
     * @example - $obNomeObjeto->recuperarTodos( 'campo1, campo2', array( "id = 123" ), nome )
     * @example - $obNomeObjeto->recuperarTodos() --> Se n�o passar par�metros reconhece como todos os registros do banco.
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function recuperarTodos( $stCampos = '*', $arClausulas = null, $stOrdem = null, array $opcoes = array())
    {
        // -- Retornar a query ao inv�s dos dados
        $opcoes['query'] = isset($opcoes['query'])?$opcoes['query']:false;
        // -- Faz join com as tabelas mapeadas nas foreing keys
        $opcoes['join'] = isset($opcoes['join'])?$opcoes['join']:false;

        $sql = "SELECT DISTINCT $stCampos FROM public.vw_subunidadeorcamentaria t1";

        $contagemTabelas = 2;
        if ($opcoes['join']) {
            if (empty($this->arChaveEstrangeira)) {
                throw new Exception("N�o existe nenhuma chave estranjeira definida para a tabela {$this->stNomeTabela}.");
            }
            $sql .= $this->gerarJoin($contagemTabelas, $opcoes['join']);
        }
        if ($opcoes['leftjoin']) {
            if (empty($this->arChaveEstrangeira)) {
                throw new Exception("N�o existe nenhuma chave estranjeira definida para a tabela {$this->stNomeTabela}.");
            }
            $sql .= $this->gerarJoin($contagemTabelas, $opcoes['leftjoin'], 'LEFT');
        }

        $sql .= $arClausulas ? " WHERE ". implode( " AND ", $arClausulas ) : "";
        $sql .= $stOrdem ? " ORDER BY $stOrdem" : "";

        if ($opcoes['query']) {
            return $sql;
        }

        $resultado = $this->carregar($sql);
        return $resultado ? $resultado : array();
    }

}//end Class
?>