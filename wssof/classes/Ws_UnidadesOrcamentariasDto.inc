<?php
/**
 * Classe de mapeamento da entidade wssof.ws_unidadesorcamentariasdto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */



/**
 * Wssof_Ws_unidadesorcamentariasdto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Ws_UnidadesOrcamentariasDto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'wssof.ws_unidadesorcamentariasdto';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
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
        'descricao' => null,
        'codigoorgao' => null,
        'exercicio' => null,
        'orgaosiorg' => null,
        'codigoorgaopai' => null,
        'orgaoid' => null,
        'snativo' => null,
        'descricaoabreviada' => null,
        'dataultimaatualizacao' => null,
        'tipoorgao' => null,
    );

}//end Class
?>