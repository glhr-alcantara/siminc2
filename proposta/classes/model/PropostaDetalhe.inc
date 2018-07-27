<?php
/**
 * Classe de mapeamento da entidade proposta.propostadetalhe
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <teste@teste.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-04-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Proposta_Model_PropostaDetalhe
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Proposta_Model_PropostaDetalhe extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proposta.propostadetalhe';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'prdid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'idoid' => array('tabela' => 'idoc', 'pk' => 'idoid'),
        'fonid' => array('tabela' => 'fonterecurso', 'pk' => 'fonid'),
        'iduid' => array('tabela' => 'identifuso', 'pk' => 'iduid'),
        'ndpid' => array('tabela' => 'naturezadespesa', 'pk' => 'ndpid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'prdid' => null,
        'ndpid' => null,
        'iduid' => null,
        'fonid' => null,
        'idoid' => null,
        'prdvalor' => null,
        'prdvalorexpansao' => null,
        'proid' => null,
        'prdstatus' => null,
    );
    
    /**
     * Montar consulta para buscar os dados do detalhe da proposta(Financeiro).
     * 
     * @param stdClass $filtro
     * @return string
     */
    public function montarSqlListaFinanceiro(stdClass $filtro) {
        # Filtros
        $where = '';
        $where .= $filtro->prdid? "\n AND prd.prdid = ". (int)$filtro->prdid: NULL;
        $where .= $filtro->noPrdid? "\n AND prd.prdid != ". (int)$filtro->noPrdid: NULL;
        $where .= $filtro->ndpid? "\n AND prd.ndpid = ". (int)$filtro->ndpid: NULL;
        
        $sql = "
            SELECT
                prd.prdid,
                prd.ndpid,
                ndp.ndpcod,
                ndp.gndcod,
                idu.iducod,
                fr.foncod,
                ido.idocod,
                prd.prdvalor,
                prd.prdvalorexpansao,
                prd.proid,
                prd.prdstatus
            FROM proposta.propostadetalhe prd
                JOIN public.naturezadespesa ndp ON prd.ndpid = ndp.ndpid
                JOIN public.idoc ido ON prd.idoid = ido.idoid
                left join public.fonterecurso fr on prd.fonid = fr.fonid
                left join public.identifuso idu on prd.iduid = idu.iduid
            WHERE
                prd.prdstatus = 'A'
                AND prd.proid = ". (int)$filtro->proid. "
                $where
        ";
        return $sql;
    }
    
    /**
     * Busca os valores financeiros de totais e expansões tanto de custeio quanto
     * de capital da proposta.
     * 
     * @param int $proid
     * @param int $noPrdid
     * @return int
     */
    public function buscarTotais(){
        $totais = array(
            'custeioValor' => 0,
            'capitalValor' => 0,
            'custeioExpansao' => 0,
            'capitalExpansao' => 0,
        );
        $sql = $this->montarSqlListaFinanceiro((object) array(
            'proid' => $this->proid,
            'noPrdid' => $this->prdid));
        $resultado = $this->carregar($sql);
        if($resultado){
            foreach($resultado as $financeiro){
                if($financeiro['gndcod'] <= 3){
                    $totais['custeioValor'] += $financeiro['prdvalor'];
                    $totais['custeioExpansao'] += $financeiro['prdvalorexpansao'];
                } else {
                    $totais['capitalValor'] += $financeiro['prdvalor'];
                    $totais['capitalExpansao'] += $financeiro['prdvalorexpansao'];
                }
            }
        }
        
        return $totais;
    }
    
    /**
     * Retorna lista de dados da consulta para buscar os dados do detalhe da proposta(Financeiro).
     * 
     * @param stdClass $filtro
     * @return array
     */
    public function buscarListaFinanceiro(stdClass $filtro) {
        $sql = $this->montarSqlListaFinanceiro($filtro);
        $resultado = $this->carregar($sql);
        $listaFinanceiro = $resultado? $resultado: [];
        return $listaFinanceiro;
    }
    
    /**
     * Retorna dados da consulta para buscar o detalhe da proposta(Financeiro).
     * 
     * @param stdClass $filtro
     * @return stdClass
     */
    public function buscarFinanceiro(stdClass $filtro) {
        $sql = $this->montarSqlListaFinanceiro($filtro);
        $resultado = $this->pegaLinha($sql);
        $financeiro = (object)($resultado? $resultado: []);
        return $financeiro;
    }

}//end Class
?>