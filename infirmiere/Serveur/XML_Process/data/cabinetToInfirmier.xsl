<?xml version="1.0" encoding="UTF-8"?>

<!--
    Document   : infirmier.xsl
    Created on : 14 octobre 2014, 08:19
    Author     : stevensm
    Description:
        Purpose of transformation follows.
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
                xmlns:ref="http://www.ujf-grenoble.fr/l3miage/cabinet"
                xmlns:act='http://www.ujf-grenoble.fr/l3miage/actes'>
                
    <xsl:output method="html"/>

    <!-- TODO customize transformation rules 
         syntax recommendation http://www.w3.org/TR/xslt 
    -->
    <xsl:variable name="actes" select="document('actes.xml', /)/act:ngap"/>
    <xsl:param name="destinedId" select="001"/>
    <xsl:template match="/">
        <html>
            <head>
                <title>infirmier.xsl</title>
                <link rel="stylesheet" type="text/css" href="css/Style.css"/>
            </head>
            <body>
           
                <script type="text/javascript" src="js/facture.js"></script>
                 <!--   <![CDATA[
                    function openFacture(prenom, nom, actes) {
                    var width  = 500;
                    var height = 300;
                    if(window.innerWidth) {
                    var left = (window.innerWidth-width)/2;
                    var top = (window.innerHeight-height)/2;
                    }
                    else {
                    var left = (document.body.clientWidth-width)/2;
                    var top = (document.body.clientHeight-height)/2;
                    }
                    var factureWindow = window.open('','facture','menubar=yes, scrollbars=yes, top='+top+', left='+left+', width='+width+', height='+height+'');
                    factureText = "Facture pour : " + prenom + " " + nom;
                    factureWindow.document.write(factureText);
                    }     
                    ]]>
                </script>  -->         
                        
                <h1>Bonjour <xsl:value-of select="ref:cabinet/ref:infirmiers/ref:infirmier[@id=$destinedId]/ref:prenom"/>  </h1>
                <h2> vous avez  <xsl:value-of select =" count(//ref:patient/ref:visite[@intervenant=$destinedId])"/> patients</h2>
                <table>
                    <tr>
                        <th> Nom </th>
                        <th> Prenom </th>
                        <th> Adresse </th>
                        <th> Actes </th>
                        <th> Facture </th>
                    </tr>
                    <xsl:apply-templates select="//ref:patient"/> 
                </table>
            </body>
        </html>
    </xsl:template>
    
    <xsl:template match="ref:patient">
        <xsl:variable name="actes" select="document('actes.xml', /)/act:ngap"/>
        <xsl:if test="ref:visite[@intervenant=$destinedId]">
            
            <tr>    
                <td>
                    <xsl:value-of select="ref:nom"/>
                </td>    
                <td>
                    <xsl:value-of select="ref:prenom"/>
                </td>
                <td>
                    <xsl:value-of select="ref:adresse/ref:numero"/>
                    <xsl:text> </xsl:text> 
                    <xsl:value-of select="ref:adresse/ref:rue"/>
                    <br/>
                    <xsl:value-of select="ref:adresse/ref:codePostal"/>
                    <xsl:text> </xsl:text> 
                    <xsl:value-of select="ref:adresse/ref:ville"/>
                </td>
                <td>
                    <ul>
                        <xsl:apply-templates select="ref:visite/ref:acte"/>
                    </ul> 
                </td>
                <td>
                    <xsl:element name="button">
                        <xsl:attribute name="onclick">
                            openFacture('<xsl:value-of select="ref:nom"/>',
                            '<xsl:value-of select="ref:prenom"/>',
                            '<xsl:for-each select="ref:visite/ref:acte">
                                <xsl:variable name="idAct" select="@id"/>
                                <xsl:value-of select="($actes/act:actes/act:acte[@id=$idAct]/@id)"/>
                                <xsl:text> </xsl:text>
                            </xsl:for-each>');
                        </xsl:attribute>  
                        Facture
                    </xsl:element>
                    <!-- <form>
                        <input type="button" name="nom" value="Facture" />
                    </form>-->
                </td>
            </tr>
            
        </xsl:if>        
    </xsl:template>
    
    
    <xsl:template match="ref:acte">
        
        <xsl:variable name="num">
            <xsl:value-of select="@id"/>
        </xsl:variable>

        <li>
            <xsl:value-of select="$actes/act:actes/act:acte[@id=$num]"/>
        </li>
                          
    </xsl:template>
</xsl:stylesheet>
