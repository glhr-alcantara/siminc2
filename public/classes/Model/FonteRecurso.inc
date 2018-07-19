<?php
/**
 * Classe de mapeamento da entidade public.fonterecurso
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-11-2017
 * @link     no link
 */


require_once APPRAIZ . 'includes/classes/Modelo_PK_Char.class.inc';


/**
 * Public_Model_Fonterecurso
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_FonteRecurso extends Modelo_PK_Char
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.fonterecurso';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'foncod',
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
        'foncod' => null,
        'fondsc' => null,
        'fonstatus' => null,
        'fonabrev' => null,
        'grfid' => null,
        'clasproporc' => null,
    );

    /**
     * Consulta se o código inserido existe e retorna a primeira coluna do primeiro registro.
     * OBS: O código que está sendo passado por parametro está sendo instanciado em Public_Controller_Fonte
     * @return Modelo_PK_Char
     */
    public function carregarPorCodigo()
    {
        $sql = "SELECT * FROM public.fonterecurso WHERE fonstatus = 'A' AND foncod ='". $this->foncod ."'";

        $resultado = $this->pegaLinha($sql);
        if($resultado){
            $this->popularDadosObjeto($resultado);
        } else {
            $this->foncod = NULL;
        }

        return $this;
    }

}
