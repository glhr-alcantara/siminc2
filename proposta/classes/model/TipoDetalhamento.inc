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
