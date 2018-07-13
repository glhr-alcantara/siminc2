<?php
/**
 * Classe de mapeamento da entidade spo.subunidadeiniciativappa
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06.07.2018
 * @link     no link
 */

/**
 * Mapeamento da entidade monitora.ptres.
 *
 * @see Modelo
 */
class Spo_Model_SubUnidadeIniciativaPpa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'spo.subunidadeiniciativappa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'suoippid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'suoippid' => null,
        'suoid' => null,
        'ippid' => null,
    );

    public function excluirPorExercicio($exercicio)
    {
        $sql = "delete from ".$this->stNomeTabela." where ippid in (select ippid from public.iniciativappa where prsano = '{$exercicio}')";
        return $this->executar($sql);
    }

    public function recuperarPorExercicio($exercicio)
    {
        $sql = "select * from ".$this->stNomeTabela." where ippid in (select ippid from public.iniciativappa where prsano = '{$exercicio}')";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach($dados as $dado){
            $dadosAgrupados[$dado['ippid']][] = $dado['suoid'];
        }
        return $dadosAgrupados;
    }
}