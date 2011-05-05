<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format" version="1.0">

  <!-- FILE: regions.xsl
       DESCRIPTION: Match all regions that are not text to display them -->

  <xsl:template match="pcGts/page/image_region|
		       pcGts/page/graphic_region|
		       pcGts/page/chart_region|
		       pcGts/page/table_region|
		       pcGts/page/vertical_separator_region|
		       pcGts/page/horizontal_separator_region">

    <!-- ID of the region, used to display id.png -->
    <xsl:variable name="data">
      <xsl:value-of select="container/data" />
    </xsl:variable>

    <!-- Regions coordinates -->
    <xsl:variable name="y1">
      <xsl:for-each select="coords/point">
	<xsl:sort select="@y" order="ascending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@y" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="x1">
      <xsl:for-each select="coords/point">
	<xsl:sort select="@x" order="ascending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@x" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>
    <!-- END OF regions coordinates -->

    <fo:block-container position="absolute">

      <xsl:attribute name="left">
	<xsl:value-of select="$x1" />px
      </xsl:attribute>
      <xsl:attribute name="top">
	<xsl:value-of select="$y1" />px
      </xsl:attribute>

      <fo:block>
	<fo:external-graphic>
	  <xsl:attribute name="src">
	    url('data:image/png;base64,<xsl:value-of select="$data" />')
	  </xsl:attribute>
	</fo:external-graphic>
      </fo:block>

    </fo:block-container>

  </xsl:template>

</xsl:stylesheet>
