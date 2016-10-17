<?hh // strict

namespace Ivyhjk\Xml\Test\Entity;

use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Entity\Value;
use Ivyhjk\Xml\Entity\Param;
use Ivyhjk\Xml\Entity\Params;
use Ivyhjk\Xml\Entity\MethodResponse;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * Test the correct workflow for <methodResponse> tag wrapper.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Test\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
/* HH_FIXME[4123] */ /* HH_FIXME[2049] */
class MethodResponseTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Test the params getter.
     *
     * @return void
     */
    public function testGetParameters() : void
    {
        $domDocument = new DOMDocument();

        $parameters = Vector{
            new Params(Vector{}, $domDocument),
            new Params(Vector{}, $domDocument)
        };

        $methodResponse = new MethodResponse($parameters, $domDocument);

        static::assertSame($parameters, $methodResponse->getParameters());
    }

    /**
     * Test the getElement method correct workflow.
     *
     * @return void
     */
    public function testGetElement() : void
    {
        $domDocument = new DOMDocument();

        $param = new Param(Vector{
            new Value(Vector{
                Map{
                    'foo' => 'bar'
                }
            }, $domDocument)
        }, $domDocument);

        $params = new Params(Vector{$param}, $domDocument);

        $methodResponse = new MethodResponse(Vector{$params}, $domDocument);

        $domDocument->appendChild($methodResponse->getElement());

        $xml = preg_replace('/\n/', '', $domDocument->saveXML());

        $expected = '<?xml version="1.0"?><methodResponse><params><param><value><struct><member><name>foo</name><value><string>bar</string></value></member></struct></value></param></params></methodResponse>';

        static::assertSame($expected, $xml);
    }

    /**
     * Test the fromNode error.
     *
     * @return void
     */
    public function testFromNodeError() : void
    {
        $node = new SimpleXMLElement('<invalidTag/>');

        $this->expectException(InvalidNodeException::class);
        MethodResponse::fromNode($node, new DOMDocument());
    }

    /**
     * Test the fromNode correct workflow.
     *
     * @return void
     */
    public function testFromNode() : void
    {
        $baseXML = preg_replace(['/>\s+</', '/\n/', '/\s+</'], ['><', '', '<'],'
            <?xml version="1.0"?>
            <methodResponse>
                <params>
                    <param>
                        <value>
                            <struct>
                                <member>
                                    <name>foo</name>
                                    <value>
                                        <string>bar</string>
                                    </value>
                                </member>
                                <member>
                                    <name>bar</name>
                                    <value>
                                        <string>baz</string>
                                    </value>
                                </member>
                            </struct>
                        </value>
                    </param>
                </params>
            </methodResponse>');

        $domDocument = new DOMDocument();
        $methodResponseNode = new SimpleXMLElement($baseXML);
        $response = MethodResponse::fromNode($methodResponseNode, $domDocument);

        $domDocument->appendChild($response->getElement());

        $decoded = preg_replace('/\n/', '', $domDocument->saveXML());

        static::assertSame($baseXML, $decoded);
    }
}
