<div class="row">
    <div class="col-md-3">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
            <tr class="text-center">
                <th>Partido</th>
                <?php
                $dadosTabela = (array)current($dadosAgrupados);
                foreach($dadosTabela as $categoria => $descricao){ ?>
                    <th><?php echo $categoria; ?></th>
                <?php } ?>
            </tr>
            </thead>
            <tbody>
            <?php $aTotal = [];
            foreach($dadosAgrupados as $agrupador => $dadosTabela){ ?>
                    <tr>
                        <td><?php echo $agrupador; ?></td>
                        <?php foreach($dadosTabela as $key => $descricao){
                            $aTotal[$key] += $descricao;
                            ?>
                            <td width="30%" class="text-right"><?php echo number_format($descricao, 0, ',', '.'); ?></td>
                        <?php } ?>
                    </tr>
            <?php } ?>
            </tbody>
            <tr>
                <th>TOTAL</th>
                <?php
                foreach($dadosTabela as $key => $descricao){ ?>
                    <th class="text-right"><?php echo number_format($aTotal[$key], 0, ',', '.'); ?></th>
                <?php } ?>
            </tr>
        </table>
    </div>
    <div class="col-md-9">
        <div class="row">
            <div class="col-md-12" style="margin-bottom: 100px;">
                <?php
                $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
                if($colors){ $grafico->setColors($colors); }
                $grafico->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->setTitulo('Valores por Partido')->setHeight(400)->gerarGrafico($dados);
                unset($grafico);
                ?>
            </div>
            <div class="col-md-6">
                <table class="table table-striped table-bordered table-hover" >
                    <thead>
                    <tr class="text-center">
                        <th>Base</th>
                        <?php
                        $dadosTabela = (array)current($dadosAgrupadosLado);
                        foreach($dadosTabela as $categoria => $descricao){ ?>
                            <th><?php echo $categoria; ?></th>
                        <?php } ?>
                    </tr>
                    </thead>
                    <tbody>
                    <?php $aTotal = [];
                    foreach($dadosAgrupadosLado as $agrupador => $dadosTabela){ ?>
                        <tr>
                            <td><?php echo $agrupador; ?></td>
                            <?php foreach($dadosTabela as $key => $descricao){
                                $aTotal[$key] += $descricao;
                                ?>
                                <td width="30%" class="text-right"><?php echo number_format($descricao, 0, ',', '.'); ?></td>
                            <?php } ?>
                        </tr>
                    <?php } ?>
                    </tbody>
                    <tr>
                        <th>TOTAL</th>
                        <?php
                        foreach($dadosTabela as $key => $descricao){ ?>
                            <th class="text-right"><?php echo number_format($aTotal[$key], 0, ',', '.'); ?></th>
                        <?php } ?>
                    </tr>
                </table>
            </div>
            <div class="col-md-6">
                <?php
                $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
                if($colors){ $grafico->setColors($colors); }
                $grafico->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->setTitulo('Valores por Base')->gerarGrafico($dadosLado);
                unset($grafico);
                ?>
            </div>
        </div>
    </div>
</div>

