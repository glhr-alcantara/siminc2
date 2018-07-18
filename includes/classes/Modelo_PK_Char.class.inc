<?php

/**
 * Implementação da classe base de modelo.
 *
 * @version $Id: Modelo.class.inc 103541 2015-10-09 20:36:35Z mariluciaqueiroz $
 * @filesource
 */

/**
 * Classe pai para as classes espelhos de banco.
 *
 * @package Simec\Db
 * @author Orion Teles de Mesquita
 * @since 12/02/2009
 */
class Modelo_PK_Char extends Modelo {

    /**
     * Função salvar
     * Método usado para inserção ou alteração de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access public
     * @author Rafael Freitas Carneiro
     * @since 18/07/2018
     */
    public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array(), $manterAspas = false) {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();

        if ($boAntesSalvar) {
            if (!$this->antesSalvar()) {
                return false;
            }
        }

        if (count($this->arChavePrimaria) > 1)
            trigger_error("Favor sobreescrever método na classe filha!");

        $stChavePrimaria = $this->arChavePrimaria[0];
        $this->validar($this->arAtributos);
        $sql = "select * from ".$this->stNomeTabela." where ".$this->arChavePrimaria[0]." = '".$this->{$stChavePrimaria}."'";
        $verificaRegistro = $this->carregar($sql);  
        if ($this->$stChavePrimaria && !$this->tabelaAssociativa && count($verificaRegistro)>0) {
            $this->alterar($arCamposNulo);
            $resultado = $this->$stChavePrimaria;
        } else {
            if ($manterAspas === false) {
                $resultado = $this->inserir($arCamposNulo);
            } else {
                $resultado = $this->inserirManterAspas($arCamposNulo);
            }
        }
        if ($resultado) {
            if ($boDepoisSalvar) {
                $this->depoisSalvar();
            }
        }
        return $resultado;
    }

    /**
     * Função _inserir
     * Método usado para inserção de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access private
     * @author Rafael Freitas Carneiro
     * @since 18/07/2018
     */
    public function inserir($arCamposNulo = array()) {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
        if (count($this->arChavePrimaria) > 1)
            trigger_error("Favor sobreescrever método na classe filha!");

        $arCampos = array();
        $arValores = array();
        $arSimbolos = array();

        $troca = array("'", "\\");
        foreach ($this->arAtributos as $campo => $valor) {
            if ($campo == $this->arChavePrimaria[0] && !$this->tabelaAssociativa){
                $arCampos[] = $campo;
                $arValores[] = trim(pg_escape_string($valor));
                continue;
            }
            if ($valor !== null) {
                if (!$valor && in_array($campo, $arCamposNulo)) 
                    continue;
                $arCampos[] = $campo;
                $valor = str_replace($troca, "", $valor);
                $arValores[] = trim(pg_escape_string($valor));
            }
        }

        if (count($arValores)) {
            $sql = " insert into $this->stNomeTabela ( " . implode(', ', $arCampos) . " )
											  values ( '" . implode("', '", $arValores) . "' )
					 returning {$this->arChavePrimaria[0]}";
            $stChavePrimaria = $this->arChavePrimaria[0];
            return $this->$stChavePrimaria = $this->pegaUm($sql);
        }
    }

    /**
     * Função _alterar
     * Método usado para alteração de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access private
     * @author Rafael Freitas Carneiro
     * @since 18/07/2018
     */
    public function alterar($arCamposNulo = array()) {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
        if (count($this->arChavePrimaria) > 1)
            trigger_error("Favor sobreescrever método na classe filha!");

        $campos = "";
        foreach ($this->arAtributos as $campo => $valor) {
            if ($valor != null) {
                if ($campo == $this->arChavePrimaria[0]) {
                    $valorCampoChave = $valor;
                    continue;
                }
                $valor = pg_escape_string($valor);
                $campos .= $campo . " = '" . $valor . "', ";
            } else {
                if (in_array($campo, $arCamposNulo)) {
                    $campos .= $campo . " = null, ";
                }
            }
        }

        $campos = substr($campos, 0, -2);

        $sql = " UPDATE $this->stNomeTabela SET $campos WHERE {$this->arChavePrimaria[0]} = '$valorCampoChave' ";

        return $this->executar($sql);
    }

}
