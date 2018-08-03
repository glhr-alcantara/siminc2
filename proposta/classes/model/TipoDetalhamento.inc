<?php
/**
 * Classe de mapeamento da entidade proposta.tipodetalhamento
 *
 * @category Class Tipo de Detalhamento
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-08-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Proposta_Model_Proposta
 *
 * @category Class Tipo de Detalhamento
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-08-2018
 * @link     no link
 */
class Proposta_Model_TipoDetalhamento extends Modelo
{
    /**
     * Constantes dos tipos de detalhamento da proposta.
     *
     * @const DESPESAS_EXECUTIVO = "Demais Despesas Discicionárias do Poder Executivo"
     * @const PAC = "PAC (Exceto prioridades e metas - Anexos LDO)"
     * @const DESPESAS_PODER_EXECUTIVO = "Obrigatórias com Demais Poderes, MPU e DPU, exceto Benefícios a Servidores"
     * @const DESPESAS_ENCARGOS_SOCIAIS = "Despesas com Benefícios a Servidores dos demais poderes, MPU e DPU"
     * @const DESPESAS_OBRIGATORIAS = "Despesas Obrigatórias sem Controle de Fluxo lançadas no SIOP pela SOF"
     * @const DESPESAS_FINANCEIRAS = "Despesas Financeiras, exceto Dívida Contratual e Mobiliária"
     *
     */
    const DESPESAS_EXECUTIVO = 1;
    const PAC = 3;
    const DESPESAS_PODER_EXECUTIVO = 6;
    const DESPESAS_ENCARGOS_SOCIAIS = 8;
    const DESPESAS_OBRIGATORIAS = 9;
    const DESPESAS_FINANCEIRAS = 11;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proposta.tipodetalhamento';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'tpdid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(

    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'tpdid' => null,
        'tpdcod' => null,
        'tpdnome' => null,
        'tpdstatus' => null,
        'tpdano' => null,
        'tpdcadastro' => null,
    );

}
