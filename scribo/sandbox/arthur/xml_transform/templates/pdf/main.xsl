<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format" version="1.0">

  <!-- FILE: main.xsl
       DESCRIPTION: produce pdf output -->

  <xsl:import href="line.xsl"/>

  <xsl:template match="/">
    <fo:root>

      <fo:layout-master-set>
	<fo:simple-page-master master-name="page" margin-left="0cm" margin-right="0cm" margin-bottom="0cm" margin-top="0cm">

	  <!-- Output will be sized with image size -->
	  <xsl:attribute name="page-width">
	    <xsl:value-of select="pcGts/page/@image_width" />px
	  </xsl:attribute>
	  <xsl:attribute name="page-height">
	    <xsl:value-of select="pcGts/page/@image_height" />px
	  </xsl:attribute>

          <fo:region-body
             margin-top="0mm" margin-bottom="0mm"
             margin-left="0mm" margin-right="0mm"/>
          <fo:region-after extent="0mm"/>

	</fo:simple-page-master>
      </fo:layout-master-set>

      <fo:page-sequence master-reference="page">

	<fo:flow flow-name="xsl-region-body">

	  <xsl:apply-imports />

	  <fo:block-container position="absolute" z-index="-3">

	    <xsl:attribute name="left">
	      0px
	    </xsl:attribute>
	    <xsl:attribute name="top">
	      0px
	    </xsl:attribute>

	    <fo:block>
	      <fo:external-graphic>
		<xsl:attribute name="src">
		  url('img/image.png')
		</xsl:attribute>
	      </fo:external-graphic>
	    </fo:block>

	  </fo:block-container>

	</fo:flow>

      </fo:page-sequence>
    </fo:root>
  </xsl:template>

</xsl:stylesheet>
