<?php
/**
 * Classe de mapeamento da entidade public.grupofonte
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTE <douglas.fontes@cultura.gov.br>
 * @license  GNU siminc.cultura.gov.br
 * @version  Release: 27-07-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_GrupoFonte
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTE <douglas.fontes@cultura.gov.br>
 * @license  GNU siminc.cultura.gov.br
 * @version  Release: 27-07-2018
 * @link     no link
 */
class Public_Model_GrupoFonte extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.grupofonte';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'grfid',
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
        'grfid' => null,
        'grfdsc' => null,
        'grfstatus' => null,
        'grfid2' => null,

    );

    /**
     * Retorna a combo de grupo fonte
     *
     * @return string
     */
    public function recuperarlistaGrupoFonte()
    {
        $sql ="SELECT
                    grfid as codigo,
                    grfid || ' - ' || grfdsc as descricao
                FROM public.grupofonte 
                WHERE
                    grfstatus = 'A'
                ORDER BY descricao
        ";
        return $sql;
    }
}
