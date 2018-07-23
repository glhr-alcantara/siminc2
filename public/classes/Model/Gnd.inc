<?php
/**
 * Classe de mapeamento da entidade public.gnd
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Gnd
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_Gnd extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.gnd';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'gndcod',
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
        'gndcod' => null,
        'gnddsc' => null,
        'gndabrev' => null,
        'gndstatus' => null,
    );

    /**
     * Recupera lista com código e descrição por gnd.
     *
     * @return string
     */
    public function recuperarlistaGnd()
    {
        $sql ="SELECT
                    gndcod AS codigo,
                    gndcod || ' - ' || gnddsc AS descricao
                FROM
                    public.gnd
                WHERE
                    gndstatus = 'A'
        ";
        return $sql;
    }
}//end Class
?>